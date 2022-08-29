// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardSpawnComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"

#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/Flipbook/GameActor/FlameActor.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include "GameSystem/FVMGameInstance.h"
#include <Kismet/GameplayStatics.h>
// Sets default values for this component's properties
UCardSpawnComponent::UCardSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardSpawnComponent::LoadResource()
{
	this->M_Owner = Cast<ACardActor>(this->GetOwner());

	if (!this->M_Owner)
		return;

	//设置生产量
	this->M_SpawnFlameValue = this->M_Owner->GetCardSpawnFlameNum();
	this->M_SpawnCount = this->M_Owner->GetCardSpawnFlameCount();
	this->M_SpawnDelay = this->M_Owner->GetCardSpawnSpeed();
	this->M_Condition.M_CurrentCardIsWeekUp = this->M_Owner->GetCardSpawnAutoWeekUp();
	this->M_Condition.M_AutoUpTime = this->M_Owner->GetCardSpawnWeekUpTime();
	this->M_Condition.M_SpawnFlameNumRate = this->M_Owner->GetCardSpawnFlameRate();

	int32 M_ResourceCount = this->M_Owner->M_ResourcePath.Num();

	if (M_ResourceCount > 0)
		this->M_UPaperFlipbook_Idle = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[0]);

	if (M_ResourceCount > 1)
		this->M_UPaperFlipbook_Spawn = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[1]);

	if (M_ResourceCount > 2)
		this->M_Resource.M_UPaperFlipbook_Idle = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[2]);

	if (M_ResourceCount > 3)
		this->M_Resource.M_UPaperFlipbook_Spawn = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[3]);

	if (M_ResourceCount > 4)
		this->M_Resource.M_UPaperFlipbook_Spawn_Up = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[4]);


	//初始化火苗的路径
	TSubclassOf<AFlipbookActorBase> L_CardProjectionItem = LoadClass<AFlipbookActorBase>(nullptr, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Resource/MyFlameActor.MyFlameActor_C'"));
	this->M_AFlipbookActorBase = L_CardProjectionItem.GetDefaultObject();

	this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);

	if (!this->M_AFlipbookActorBase)
	{
		UE_LOG(LogTemp, Error, TEXT("卡片投射物加载失败!请检查路径和末尾的\"_C\""));
		return;
	}
}

void UCardSpawnComponent::Spawn(const float& DeltaTime, const FVector& _Location)
{
	//如果游戏结束则不能在生成
	if (!AGameMapInstance::GetGameMapInstance()->IsInGame)
		return;

	//时间累计
	if (!this->InSpawn)
		this->LTime += DeltaTime;
	else
		this->LTime = 0.f;

	//生成模式冷却
	if (LTime >= this->M_SpawnDelay)
	{
		this->LTime = 0.f;
		this->LSpawnTime = 0.f;
		this->SpawnItem = this->M_SpawnCount;
		this->InSpawn = true;
		this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Spawn);
	}
}

void UCardSpawnComponent::BeginSpawnItem(const float& DeltaTime)
{
	//投射物模式启动
	if (this->InSpawnItem && this->SpawnItem > 0)
	{
		//首次投射物是否已经生成
		if (this->FirstSpawnEnd)
		{
			//首次投射物已经生成完毕，现在是生成剩余投射物
			this->LSpawnTime += DeltaTime;
			if (this->LSpawnTime >= this->SpawnDelay)
			{
				//生成投射物
				this->SpawnItems();
				//时间归零
				this->LSpawnTime = 0;
				//总个数减一
				this->SpawnItem--;
			}
		}
		else {
			//生成首次投射物
			this->LSpawnTime += DeltaTime;
			if (this->LSpawnTime >= this->M_FirstSpawnDelay)
			{
				this->FirstSpawnEnd = true;
				//总个数减一
				this->SpawnItem--;
				//生成投射物
				this->SpawnItems();
				//时间归零
				this->LSpawnTime = 0.f;
			}
		}

		//如果所有投射物生成完毕则关闭投射模式
		if (this->SpawnItem == 0)
		{
			this->InSpawnItem = false;
			this->FirstSpawnEnd = false;
		}
	}
}

void UCardSpawnComponent::UpdateSpawnAnimation()
{
	//进入攻击模式
	if (InSpawn)
	{
		if (this->M_Owner->AnimationIsEnd())
		{
			this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);

			//时间记录
			this->LTime = 0.f;
			//生成投射物的延迟
			this->LSpawnTime = 0.f;
			//攻击模式
			this->InSpawn = false;
		}

		//开启投射物模式
		this->InSpawnItem = true;
	}
}

void UCardSpawnComponent::InitCondition()
{
	//时间记录
	this->LTime = 0.f;
	//生成投射物的延迟
	this->LSpawnTime = 0.f;
	//攻击模式
	this->InSpawn = false;
	//投射物生成个数
	this->SpawnItem = 0;
}


// Called when the game starts
void UCardSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_Owner = Cast<ACardActor>(this->GetOwner());

	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("获取卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardSpawnComponent::SpawnItems()
{
	//生成投射物
	AFlameActor* const NewSpawn = this->GetWorld()->SpawnActor<AFlameActor>(this->M_AFlipbookActorBase->GetClass(), this->M_Owner->GetActorTransform());
	//新生成的对象设置自定义拥有者(CardActor)
	NewSpawn->SetFlameValue(this->M_SpawnFlameValue);
}


// Called every frame
void UCardSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//游戏暂停则不在生产
	if (this->M_Owner->GetGameIsPause())
		return;

	//卡片休息
	if (!this->M_Owner->GetCardDay() && this->M_Owner->GetMapDay())
		return;

	//卡片生命值不为0
	if (this->M_Owner->GetCurrentHP() > 0.f)
	{
		//自动生长时间计时
		if (this->M_Condition.M_CurrentCardIsWeekUp)
		{
			this->M_Condition.M_AutoUpCurrentTime += DeltaTime;
			if (this->M_Condition.M_AutoUpCurrentTime >= this->M_Condition.M_AutoUpTime)
			{
				this->M_Condition.M_CurrentCardIsWeekUp = false;
				//开启生长模式
				this->InWeekUp = true;
				//设置属性
				this->M_SpawnFlameValue = this->M_SpawnFlameValue * this->M_Condition.M_SpawnFlameNumRate;

				//播放生长动画
				this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_Resource.M_UPaperFlipbook_Spawn_Up);
				//动画重新赋值
				//默认动画
				this->M_UPaperFlipbook_Idle = this->M_Resource.M_UPaperFlipbook_Idle;
				//生产动画
				this->M_UPaperFlipbook_Spawn = this->M_Resource.M_UPaperFlipbook_Spawn;
				//初始化生产
				this->InitCondition();
				//播放生长音效
				UFVMGameInstance::PlayBGM_S_Static("WeekUp", "ItemAudio");
			}
		}

		//进入生长模式
		if (this->InWeekUp)
		{
			//当生长动画播放完毕之后
			if (this->M_Owner->AnimationIsEnd())
			{
				this->InWeekUp = false;
				this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);
			}
		}
		else
		{
			//关闭了生长->恢复正常
			this->Spawn(DeltaTime, FVector());
			this->UpdateSpawnAnimation();
			this->BeginSpawnItem(DeltaTime);
		}
	}



}

