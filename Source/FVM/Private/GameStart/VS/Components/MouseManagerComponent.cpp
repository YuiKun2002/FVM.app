// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/PlayerView.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"


#include "GameStart/VS/UI/UI_MouseRoundUpTip.h"
#include "GameStart/VS/UI/UI_MouseRound.h"
#include "GameStart/VS/UI/UI_MouseBoss_Slider.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Math/RandomStream.h>
#include <Kismet/KismetMathLibrary.h>


// Sets default values for this component's properties
UMouseManagerComponent::UMouseManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// 
}


// Called when the game starts
void UMouseManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UMouseManagerComponent::InitMouseLineNotify()
{
	AGameMapInstance* T_AGameMapInstance = AGameMapInstance::GetGameMapInstance();

	//如果成功获取地图实例和地图管理器
	if (T_AGameMapInstance && T_AGameMapInstance->M_UGameMapStructManager)
	{
		//初始化线路通知
		for (int32 Row = 0; Row < T_AGameMapInstance->M_UGameMapStructManager->M_Data.M_EnableRowMesheCount; Row++)
		{
			FMouseLineNotify T_NewMouseLineNotify;
			//设置线路
			T_NewMouseLineNotify.M_Line = Row;
			//设置坐标
			T_NewMouseLineNotify.M_CurrentLineLocation = T_AGameMapInstance->M_MesheControllComponent->M_LineLastLocation[Row] + FVector(-1.1f, 90.f, 0.f);
			this->M_FMouseLineNotify.Emplace(T_NewMouseLineNotify);
		}
		UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseLineNotify]:初始化老鼠线路通知"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseLineNotify]:地图实例或者地图管理器无效"));
	}
}

void UMouseManagerComponent::InitMouseStructManager()
{
	UFVMGameInstance* L_UFVMGameInstance = UFVMGameInstance::GetFVMGameInstance();

	if (L_UFVMGameInstance)
	{
		//获取地图存档
		UGameMapStructManager* _L_UGameMapStructManager = L_UFVMGameInstance->GetGameMapStructManager();

		if (!_L_UGameMapStructManager)
		{
			UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:地图存档获取失败!初始化失败!"));
			return;
		}

		//加载老鼠配置
		if (L_UFVMGameInstance->LoadMouseStructManager(_L_UGameMapStructManager->M_Data.M_MouseStructConfigName))
		{
			UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseStructManager]:老鼠配置加载成功!并且打印老鼠结构"));
			this->M_UMouseStructManager = L_UFVMGameInstance->GetMouseStructManager();
			//打印结构
			L_UFVMGameInstance->PrintGameMouseStruct(this->M_UMouseStructManager);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseStructManager]:老鼠配置加载失败!将自动加载默认老鼠配置"));
			//如果老鼠配置加载失败（则使用默认的老鼠配置）
			L_UFVMGameInstance->InitGameMouseStructManager();
		}
	}
}

void UMouseManagerComponent::InitMouseRoundTime()
{
	//如果老鼠的结构是空的则之间结束游戏
	if (!this->M_UMouseStructManager)
	{
		UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:当前老鼠的配置无效!"));
		AGameMapInstance::GetGameMapInstance()->GameOver();
		return;
	}

	if (this->M_UMouseStructManager->M_MouseRound.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:所有回合全部执行完毕"));
		AGameMapInstance::GetGameMapInstance()->GameOver();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:初始化定时器"));

	//获取当前的回合
	FMouseRound& CurrentRound = this->M_UMouseStructManager->M_MouseRound[0];

	//获取当前回合需要生成的老鼠最大个数
	this->CurrentRoundMouseMax = this->M_UMouseStructManager->M_MouseRound[0].M_MouseMax;

	//设置老鼠回合开始的定时器
	this->GetWorld()->GetTimerManager().SetTimer(this->M_TimeHandle_MouseRounBegin, [&]() {

		//回合开始
		this->MouseRoundBegin();

		//下一个回合的定时器设置（当老鼠数量为0时自动下一个回合,否则当老鼠全部生成完毕之后一定时间后自动下一个回合）
		this->GetWorld()->GetTimerManager().SetTimer(this->M_TimeHandle_AutoInMouseRound, [&]() {

			//UE_LOG(LogTemp, Warning, TEXT("当前老鼠:%d===存活:%d"), this->CurrentRoundSpawnMouseCount, this->M_CurrentExistMouseCount);

			//启用了当老鼠为0时自动进入下一回合 当老鼠全部生成完毕 并且 剩余老鼠为0
			if (CurrentRound.M_IsEnableMouseCountZero && this->CurrentRoundSpawnMouseCount == CurrentRound.M_MouseMax && this->M_CurrentExistMouseCount == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:当前回合剩余老鼠为0"));

				//移除下一个回合的定时器
				this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_AutoInMouseRound);

				UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:[%.2f]秒之后开启进入下一个回合的定时器"), CurrentRound.M_RoundEndDelay);

				//设置老鼠回合结束的定时器（一定延迟之后立刻进入下一个回合）
				this->GetWorld()->GetTimerManager().SetTimer(this->M_TimeHandle_MouseRounEnd, [&]() {

					//回合结束
					this->MouseRoundEnd();

					//自动进入下一个回合
					this->InNextRound();

					}, CurrentRound.M_RoundEndDelay, false);
			}

			//关闭了当老鼠为0,当剩余老鼠不为零，但是老鼠已经全部生成完毕
			if ((!CurrentRound.M_IsEnableMouseCountZero) && this->CurrentRoundSpawnMouseCount == CurrentRound.M_MouseMax && this->M_CurrentExistMouseCount >= 0) {

				//移除下一个回合的定时器
				this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_AutoInMouseRound);

				UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::InitMouseRoundTime]:[%.2f]秒之后进入下一回合"), CurrentRound.M_NextRoundTime);

				//设置老鼠回合结束的定时器（一定延迟后继续计时一段时间然后进入下一个回合）
				this->GetWorld()->GetTimerManager().SetTimer(this->M_TimeHandle_MouseRounEnd, [&]() {

					//回合结束
					this->MouseRoundEnd();

					//自动进入下一个回合
					this->InNextRound();

					}, CurrentRound.M_NextRoundTime + CurrentRound.M_RoundEndDelay, false);
			}

			}, 1.f, true);

		//设置老鼠在当前回合自动生成的定时器
		this->GetWorld()->GetTimerManager().SetTimer(this->M_TimeHandle_AutoSpawnMouse, [&]()
			{
				//生成老鼠
				this->SpawnMouse();

				//更新老鼠UI
				if (this->M_UUI_MouseRound)
				{
					//更新进度条
					this->M_UUI_MouseRound->UpdateMouseRoundSlider(this->CurrentRoundSpawnMouseCount, this->CurrentRoundMouseMax);
					//更新回合文字
					this->M_UUI_MouseRound->UpdateMouseRoundText(M_CurrentRound);
				}

			}, CurrentRound.M_SpawnTime, true);

		//新回合开始初始化RoundBegin
		}, CurrentRound.M_RoundBeginDelay, false);

}

void UMouseManagerComponent::ClearAllTimeHandle()
{
	this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_MouseRounBegin);
	this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_MouseRounEnd);
	this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_AutoInMouseRound);
	this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_AutoSpawnMouse);
}

void UMouseManagerComponent::MouseRoundBegin()
{
	UE_LOG(LogTemp, Warning, TEXT("新的回合开始"));
	//记录回合
	this->M_CurrentRound++;
}

void UMouseManagerComponent::MouseRoundEnd()
{
	//回合结束移除第一个回合
	this->M_UMouseStructManager->M_MouseRound.RemoveAt(0);
	//进入下一个回合当前回合生成老鼠归零
	this->CurrentRoundSpawnMouseCount = 0;

	UE_LOG(LogTemp, Warning, TEXT("当前回合结束,还剩余[%d]个回合"), this->M_UMouseStructManager->M_MouseRound.Num());
}

void UMouseManagerComponent::InNextRound()
{
	//移除所有定时器
	this->ClearAllTimeHandle();

	//重新设置生成老鼠的索引
	this->M_CurrentSpawnMouseIndex = 0;

	//如果所有回合全部结束，那么游戏结束
	if (!this->M_UMouseStructManager->M_MouseRound.Num())
	{
		AGameMapInstance::GetGameMapInstance()->GameOver();
		UE_LOG(LogTemp, Warning, TEXT("游戏结束"));
		return;
	}

	//判断当前回合是否需要Key
	if (this->M_UMouseStructManager->M_MouseRound[0].Key)
	{
		//获取玩家背包数据判断玩家是否有资格进入，如果没有资格则游戏结束，否则显示让玩家自行选择

		//加载选择UI(玩家必须有资格才能显示该UI，否则直接游戏胜利)
		APlayerView::GetPlayerViewInstance()->DisEnableTouch();
		AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->GamePause();

		UUI_MouseRoundUpTip* LUI_MouseRoundUpTip = CreateWidget<UUI_MouseRoundUpTip>(this->GetWorld(), LoadClass<UUI_MouseRoundUpTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundUpTips.UI_MouseRoundUpTips_C'")));
		LUI_MouseRoundUpTip->AddToViewport();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("进入下一个回合"));

	this->InitNextRound();

	//播放音效
	UFVMGameInstance::PlayBGM_S_Static("MouseComing", "ItemAudio");
}

void UMouseManagerComponent::InitNextRound()
{
	//创建UI显示
	UUserWidget* MouseRoundTip = CreateWidget<UUserWidget>(this->GetWorld(), LoadClass<UUserWidget>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/Tips/UI_MouseRoundTips.UI_MouseRoundTips_C'")));
	MouseRoundTip->AddToViewport();

	//重新初始化定时器
	this->InitMouseRoundTime();
}

void UMouseManagerComponent::SpawnMouse()
{
	//将现在的时间作为随机的种子
	FDateTime Time;
	FRandomStream RandomInt(Time.Now().GetTimeOfDay().GetSeconds());
	//随机在一个范围内生成一个值作为生成老鼠的个数
	int32 _SpawnMouseCount = RandomInt.RandRange(this->M_UMouseStructManager->M_MouseRound[0].M_MouseMinCount, this->M_UMouseStructManager->M_MouseRound[0].M_MouseMaxCount);
	//判断当前生成的个数是否大于预定个数
	bool ExceedMouseMax = (_SpawnMouseCount + this->CurrentRoundSpawnMouseCount) > this->M_UMouseStructManager->M_MouseRound[0].M_MouseMax ? true : false;
	//创建老鼠的个数
	int32 _CreateMouseCount = 0;
	//判断当前生成的老鼠的总和
	if (ExceedMouseMax)
	{
		//设置当前生产的老鼠个数
		this->CurrentRoundSpawnMouseCount = this->M_UMouseStructManager->M_MouseRound[0].M_MouseMax;
		//当前应该创建x只老鼠
		int32 CurrentMouseNum = (this->CurrentRoundSpawnMouseCount + _SpawnMouseCount) - this->M_UMouseStructManager->M_MouseRound[0].M_MouseMax;
		//老鼠存活个数
		this->M_CurrentExistMouseCount += CurrentMouseNum;
		//得到最后生成的老鼠个数
		_CreateMouseCount = CurrentMouseNum;
	}
	else {
		//临时每次只生成一只老鼠
		this->CurrentRoundSpawnMouseCount += _SpawnMouseCount;
		this->M_CurrentExistMouseCount += _SpawnMouseCount;
		//得到需要生成的老鼠个数
		_CreateMouseCount = _SpawnMouseCount;
	}

	//打印日志
	UE_LOG(LogTemp, Warning, TEXT("生成了%d个新的老鼠,%s最大个数%s"), _SpawnMouseCount, ExceedMouseMax ? TEXT("超过") : TEXT("未超过"), ExceedMouseMax ? TEXT("自动将老鼠生成个数调整到最大") : TEXT("等待下一次生成"));

	FDateTime LTime;
	FRandomStream LRandomInt(LTime.Now().GetTimeOfDay().GetSeconds());

	//将老鼠资源加载到场景
	float YOffset = 0.f, Layer = 0.f;
	//初始化偏移 (每一行的偏移量)
	TArray<float> OffSet;
	OffSet.Init(0.f, this->M_FMouseLineNotify.Num());

	//每一行老鼠的层叠
	TArray<float> LearyOffSet;
	LearyOffSet.Init(0.f, this->M_FMouseLineNotify.Num());


	//开始生成老鼠
	for (int32 SpawnMouseIndex = 0, CurrentLine = 0, Leary = 0; SpawnMouseIndex < _CreateMouseCount; SpawnMouseIndex++)
	{
		//如果还有老鼠则创建存在的老鼠
		if (this->M_CurrentSpawnMouseIndex < this->M_UMouseStructManager->M_MouseRound[0].M_MouseLineSetting.Num())
		{
			//获取到当前老鼠的名称
			const FString& MouseName = this->M_UMouseStructManager->M_MouseRound[0].M_MouseLineSetting[this->M_CurrentSpawnMouseIndex].M_MouseRefPath;

			//创建老鼠
			AMouseActor* CurrentMouse = this->CreateNewMouse(MouseName);

			//设置基本属性（设置老鼠的线路）
			const int32& _RanIndex = RandomInt.RandRange(0, this->M_UMouseStructManager->M_MouseRound[0].M_MouseLineSetting[this->M_CurrentSpawnMouseIndex].M_MouseSpawnLine.Num() - 1);
			this->MouseAttrSetting(CurrentMouse, OffSet, LearyOffSet, this->M_UMouseStructManager->M_MouseRound[0].M_MouseLineSetting[this->M_CurrentSpawnMouseIndex].M_MouseSpawnLine[_RanIndex]);

			CurrentLine = CurrentMouse->GetMouseLine().Row;

			this->M_CurrentSpawnMouseIndex++;
		}
		else
		{
			//当没有老鼠了，就用默认老鼠替代
			AMouseActor* CurrentMouse = this->CreateNewMouse("Mouse_0");

			//随机线路
			const int32& _RanIndex = RandomInt.RandRange(0, this->M_FMouseLineNotify.Num() - 1);

			this->MouseAttrSetting(CurrentMouse, OffSet, LearyOffSet, _RanIndex);

			CurrentLine = CurrentMouse->GetMouseLine().Row;
		}

	}

	//如果当前生成的老鼠大于或者等于预定最大个数
	if (this->CurrentRoundSpawnMouseCount == this->M_UMouseStructManager->M_MouseRound[0].M_MouseMax)
	{
		//如果老鼠生成数量和预定数量一样或者大于，则停止继续生成老鼠
		this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimeHandle_AutoSpawnMouse);

		//是否开启老鼠归0进入下一波
		bool IsEnableAutoNext = this->M_UMouseStructManager->M_MouseRound[0].M_IsEnableMouseCountZero;
		//当前字符串描述的是关闭老鼠归零自动进入下一波
		FString TimeInNext(FString::FormatAsNumber(this->M_UMouseStructManager->M_MouseRound[0].M_NextRoundTime) + TEXT("秒之后自动进入下一回合"));
		UE_LOG(LogTemp, Warning, TEXT("当前回合老鼠全部生成完毕,%s"), IsEnableAutoNext ? TEXT("当前生成的所有老鼠全部被消灭之后自动进入下一波") : *TimeInNext);
	}
}

void UMouseManagerComponent::MouseAttrSetting(AMouseActor* _CurrentMouse, TArray<float>& _Offset, TArray<float>& _OffsetLayer, const int32& _Line)
{
	if (_CurrentMouse)
	{
		//设置线路
		_CurrentMouse->SetMouseLine(_Line);
		//设置位置
		_CurrentMouse->SetActorRelativeLocation(this->M_FMouseLineNotify[_CurrentMouse->GetMouseLine().Row].M_CurrentLineLocation + FVector(-_OffsetLayer[_CurrentMouse->GetMouseLine().Row], _Offset[_CurrentMouse->GetMouseLine().Row], 0.f));
		//加上老鼠在蓝图中设置的偏移
		_CurrentMouse->AddActorLocalOffset(_CurrentMouse->M_OffsetPosition);
		//当前行的偏移量增加
		_Offset[_CurrentMouse->GetMouseLine().Row] += 15.f;
		//当前行的层叠增加
		_OffsetLayer[_CurrentMouse->GetMouseLine().Row] += 0.1f;
		//重设层叠
		if (_OffsetLayer[_CurrentMouse->GetMouseLine().Row] > 2.f)
			_OffsetLayer[_CurrentMouse->GetMouseLine().Row] = 0.f;
	}
}

AMouseActor* UMouseManagerComponent::CreateNewMouse(const FString& _MouseName, const FVector& _Location, const FRotator& _Rotation, const FVector& _Scale)
{
	//对象的路径
	FString MousePath("Blueprint'/Game/Resource/BP/GameStart/Item/Mouse/" + _MouseName + "." + _MouseName + "_C'");
	//Actor位置
	FTransform MouseTrans;
	MouseTrans.SetLocation(_Location);
	MouseTrans.SetRotation(_Rotation.Quaternion());
	MouseTrans.SetLocation(_Scale);
	return this->GetWorld()->SpawnActor<AMouseActor>(LoadClass<AMouseActor>(0, *MousePath), MouseTrans);
}

// Called every frame
void UMouseManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UMouseManagerComponent::PrintMouseLineNotify(const FMouseLineNotify& _FMouseLineNotify)
{
	UE_LOG(LogTemp, Warning, TEXT("-----------------------------------FMouseLineNotify-----------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("当前线路坐标:X[%.2f] Y[%.2f] Z[%.2f]"), _FMouseLineNotify.M_CurrentLineLocation.X, _FMouseLineNotify.M_CurrentLineLocation.Y, _FMouseLineNotify.M_CurrentLineLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("通知器线路:[%d]线"), _FMouseLineNotify.M_Line + 1);
	UE_LOG(LogTemp, Warning, TEXT("-----------------------------------FMouseLineNotify-----------------------------------"));
}

void UMouseManagerComponent::ShowMouseUI()
{
	this->M_UUI_MouseRound = CreateWidget<UUI_MouseRound>(this->GetWorld(), LoadClass<UUI_MouseRound>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UIMouseRound.UIMouseRound_C'")));
	//设置属性


	this->M_UUI_MouseRound->AddToViewport();
}

void UMouseManagerComponent::RemoveMouseUI()
{
	if (this->M_UUI_MouseRound && this->M_UUI_MouseRound->IsInViewport())
	{
		this->M_UUI_MouseRound->RemoveFromParent();
	}
}

bool UMouseManagerComponent::IsEndRound()
{
	return (this->M_UMouseStructManager->M_MouseRound.Num() == 1);
}

void UMouseManagerComponent::GameStart()
{
	//初始化老鼠线路通知
	this->InitMouseLineNotify();
	//初始化老鼠配置
	this->InitMouseStructManager();
	//初始化计时器
	this->InitMouseRoundTime();
	//初始化UI
	this->ShowMouseUI();
}


void UMouseManagerComponent::GameOver()
{
	this->ClearAllTimeHandle();

	this->M_FMouseLineNotify.Empty();

	UE_LOG(LogTemp, Warning, TEXT("[UMouseManagerComponent::GameOver]:游戏结束"));
}

