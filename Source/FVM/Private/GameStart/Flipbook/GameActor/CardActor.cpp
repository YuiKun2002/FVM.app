
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI/UI_Card.h"
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Components/BoxComponent.h>

#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameStart/VS/GameMapInstance.h"


void ACardActor::SetCardActor(const FItemCard& _CardData)
{
	this->SetCardHP(_CardData.M_CardHP, 0.f);
	this->SetCardType(_CardData.M_ELineType, _CardData.M_CardLayer);
	this->SetCardGrade(this->GetCardGrade(_CardData.ItemName));
	this->M_CardName = _CardData.ItemName;
	this->M_CardDay = _CardData.M_CardDay;
	this->M_CardID = _CardData.M_CardLayer;
}

FLine ACardActor::GetLine()
{
	return this->M_Line;
}

//计算射线坐标
void CalculateLineTracePosition(ACardActor* _CardActor, FLineTraceSetting& _LineTraceSetting, TArray<FLineTraceSetting>& _LineTraceSettingOut)
{
	//基本坐标
	FVector BaseLocation = _CardActor->GetActorLocation();
	float TargetLY = _CardActor->GetUIMapMesh()->GetMapMeshCurrentLeft(_CardActor->GetLine().Row).Y - 45.f;
	float TargetRY = _CardActor->GetUIMapMesh()->GetMapMeshCurrentRight(_CardActor->GetLine().Row).Y + 45.f;
	FVector LocationLBegin = _CardActor->GetUIMapMesh()->GetMapMeshCurrentLeft(_CardActor->GetLine().Row);
	FVector LocationREnd = _CardActor->GetUIMapMesh()->GetMapMeshCurrentRight(_CardActor->GetLine().Row);
	FVector Top = _CardActor->GetUIMapMesh()->GetMapMeshCurrentTop(_CardActor->GetLine().Col);
	FVector Bottom = _CardActor->GetUIMapMesh()->GetMapMeshCurrentBottom(_CardActor->GetLine().Col);

	FLineTraceSetting _Item;

	switch (_LineTraceSetting.M_ELineTracePosition)
	{
	case ELineTracePosition::E_Left:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetLY, BaseLocation.Z);
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_Right:
	{
		FVector LocationEnd = FVector(BaseLocation.X, TargetRY, BaseLocation.Z);
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = LocationEnd;
	}
	break;
	case ELineTracePosition::E_LeftRight_M:
	{
		LocationLBegin.Y -= 45.f;
		LocationREnd.Y += 45.f;
		_Item.M_BeginLocation = LocationLBegin;
		_Item.M_EndLocation = LocationREnd;
	}
	break;
	case ELineTracePosition::E_Top:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = Top;
	}
	break;
	case ELineTracePosition::E_Bottom:
	{
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;
		_Item.M_BeginLocation = BaseLocation;
		_Item.M_EndLocation = Bottom;
	}
	break;
	case ELineTracePosition::E_TopBottom_M:
	{
		Top.X = BaseLocation.X;
		Top.Y = BaseLocation.Y;
		Top.Z += 45.f;
		Bottom.X = BaseLocation.X;
		Bottom.Y = BaseLocation.Y;
		Bottom.Z -= 45.f;
		_Item.M_BeginLocation = Top;
		_Item.M_EndLocation = Bottom;
	}
	break;
	}

	_LineTraceSettingOut.Emplace(_Item);
}


void ACardActor::SetCardData_ATK(const FItemCardATK& _ATK_Data)
{
	//_BaseATK + (_BaseATK / 2.f) * _ATKRate * ((float)(_CardGrade));

	//设置基础攻击力
	float ATK = _ATK_Data.M_CardATKBase;

	//计算射线位置
	for (auto Settings : _ATK_Data.M_LineTraceSettings)
	{
		CalculateLineTracePosition(this, Settings, this->M_FCardActor_ATK.M_LineTraceSettings);
	}

	//判断是否有攻击力提升功能
	if (_ATK_Data.M_ECardUpGradeType == ECardUpGradeType::E_Atk_Up)
	{
		//this->SetCardATK(_ATK_Data.M_CardATKBase, _ATK_Data.M_CardATKRate, this->GetCardGrade(_ATK_Data.ItemName));
		//攻击力计算
		ATK = _ATK_Data.M_CardATKBase + (_ATK_Data.M_CardATKBase / 2.f) * _ATK_Data.M_CardATKRate * ((float)(this->GetCardGrade(_ATK_Data.ItemName))) * _ATK_Data.M_M_ECardUpGradeUpRate;
	}

	//设置最终攻击力
	this->SetCardATK(ATK, _ATK_Data.M_CardATKRate, this->GetCardGrade(_ATK_Data.ItemName));
	this->SetCardSecondATK(_ATK_Data.M_SputteringATKRate);

	//设置攻击速度
	this->SetCardAttackSpeed(_ATK_Data.M_ShootSpeed);
	this->SetCardFirstAttackSpeed(_ATK_Data.M_FirstShootSpeed);
	this->SetCardAttackProjectionSpeed(_ATK_Data.M_ShootDelay);
	this->SetCardAttackItemCount(_ATK_Data.M_ProjectionItemCount);

	this->SetCardActor(_ATK_Data);
}

void ACardActor::SetCardData_Defence(const FItemCardDefence& _Defence_Data)
{
	//_HP + _HP * _HPUpRate * this->GetCardGrade(this->M_CardName);
	this->SetCardActor(_Defence_Data);

	float HP = _Defence_Data.M_CardHP;

	if (_Defence_Data.M_ECardUpGradeType == ECardUpGradeType::E_Defence_Up)
	{
		HP = _Defence_Data.M_CardHP + _Defence_Data.M_CardHP * _Defence_Data.M_DefenceUpRate * this->GetCardGrade(this->M_CardName);
	}
	//重新设置卡片生命值
	this->SetCardHP(HP, 0.f);
}

void ACardActor::SetCardData_Spawn(const FItemCardSpawn& _Spwan_Data)
{
	//设置生产速度
	this->M_FCardActor_Spawn.M_SpawnSpeed = _Spwan_Data.M_SpawnSpeed;

	//判断是否有生产速度功能
	if (_Spwan_Data.M_ECardUpGradeType == ECardUpGradeType::E_SpawnSpeed_Up)
	{
		this->M_FCardActor_Spawn.M_SpawnSpeed = _Spwan_Data.M_SpawnSpeed - _Spwan_Data.M_SpawnSpeed * _Spwan_Data.M_SpawnSpeedRate * _Spwan_Data.M_CardGrade * _Spwan_Data.M_M_ECardUpGradeUpRate;
	}

	this->M_FCardActor_Spawn.M_SpawnCount = _Spwan_Data.M_FlameNum;
	this->M_FCardActor_Spawn.M_SpawnFlameCount = _Spwan_Data.M_SpawnNum;
	this->M_FCardActor_Spawn.M_SpawnCardIsAutoUp = _Spwan_Data.M_EnableWeekUp;
	this->M_FCardActor_Spawn.M_SpawnCardUpTime = _Spwan_Data.M_WeekUpTime;
	this->M_FCardActor_Spawn.M_SpawnCardFlameNumRate = _Spwan_Data.M_SpawnFlameNumRate;
	this->SetCardActor(_Spwan_Data);
}

void ACardActor::SetCardData_Function(const FItemCardFunction& _Function_Data)
{
	this->M_FCardActor_Function.M_CardFunc_Data = _Function_Data;
	this->SetCardActor(_Function_Data);
}

float ACardActor::GetCardSpawnSpeed()
{
	return this->M_FCardActor_Spawn.M_SpawnSpeed;
}

int32 ACardActor::GetCardSpawnFlameNum()
{
	return this->M_FCardActor_Spawn.M_SpawnCount;
}

int32 ACardActor::GetCardSpawnFlameCount()
{
	return this->M_FCardActor_Spawn.M_SpawnFlameCount;
}

bool ACardActor::GetCardSpawnAutoWeekUp()
{
	return this->M_FCardActor_Spawn.M_SpawnCardIsAutoUp;
}

float ACardActor::GetCardSpawnWeekUpTime()
{
	return this->M_FCardActor_Spawn.M_SpawnCardUpTime;
}

float ACardActor::GetCardSpawnFlameRate()
{
	return this->M_FCardActor_Spawn.M_SpawnCardFlameNumRate;
}

FItemCardFunction ACardActor::GetCardFunctionData()
{
	return M_FCardActor_Function.M_CardFunc_Data;
}

void ACardActor::SetCardGrade(const int32& _CardGrade)
{
	if (_CardGrade > 3 && _CardGrade < 17)
	{
		FString _GradePath = FString("PaperFlipbook'/Game/Resource/Texture/CardGrade/Grade_Anim/F_CardGrade") + FString::FormatAsNumber(_CardGrade) + (".F_CardGrade") + FString::FormatAsNumber(_CardGrade) + ("'");
		this->M_CardGradeFlip->SetFlipbook(LoadObject<UPaperFlipbook>(0, *_GradePath));
	}
	else {
		this->M_CardGradeFlip->SetHiddenInGame(true);
	}
}

void ACardActor::SetCardType(const ELineType& _Type, const int32& _CardID)
{
	this->M_ELineType = _Type;
	this->M_CardID = _CardID;
}

void ACardActor::SetCardATK(const float& _BaseATK, const float& _ATKRate, const int32& _CardGrade)
{
	//后面两个是 技能书  真爱谱  神食谱 + 0.f + 0.f + 0.f;

	this->M_FCardActor_ATK.M_ATK = _BaseATK;// +(_BaseATK / 2.f) * _ATKRate * ((float)(_CardGrade));
	//UE_LOG(LogTemp, Error, TEXT("当前卡片攻击力:%.2f;基础攻击力%.2f;加成比率%.2f;等级%d"), this->M_ATK, _BaseATK, _ATKRate, _CardGrade);
}

float ACardActor::GetCardATK()
{
	return this->M_FCardActor_ATK.M_ATK;
}

void ACardActor::SetCardSecondATK(const float& _ATKRate)
{
	this->M_FCardActor_ATK.M_SecondATK = M_FCardActor_ATK.M_ATK * _ATKRate;
}

float ACardActor::GetCardSecondATK()
{
	return this->M_FCardActor_ATK.M_SecondATK;
}

void ACardActor::SetCardAttackSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_ShootSpeed = _Value;
}

float ACardActor::GetCardAttackSpeed()
{
	return this->M_FCardActor_ATK.M_ShootSpeed;
}

void ACardActor::SetCardFirstAttackSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_FirstShootSpeed = _Value;
}

float ACardActor::GetCardFirstAttackSpeed()
{
	return this->M_FCardActor_ATK.M_FirstShootSpeed;
}

void ACardActor::SetCardAttackProjectionSpeed(float _Value)
{
	this->M_FCardActor_ATK.M_ShootDelay = _Value;
}

float ACardActor::GetCardAttackProjectionSpeed()
{
	return this->M_FCardActor_ATK.M_ShootDelay;
}

void ACardActor::SetCardAttackItemCount(int32 _Value)
{
	this->M_FCardActor_ATK.M_ProjectionItemCount = _Value;
}

int32 ACardActor::GetCardAttackItemCount()
{
	return this->M_FCardActor_ATK.M_ProjectionItemCount;
}

TArray<FLineTraceSetting> ACardActor::GetLineTraceSetting()
{
	return this->M_FCardActor_ATK.M_LineTraceSettings;
}

float ACardActor::GetCurrentHP()
{
	return this->M_FCardActor_HP.M_Life;
}

float ACardActor::GetTotalHP()
{
	return this->M_FCardActor_HP.M_TotalLife;
}

void ACardActor::SetCardHP(const float& _HP, float _HPUpRate)
{
	this->M_FCardActor_HP.M_TotalLife = _HP;
	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
}

void ACardActor::SetCardCurrentHP(const float& _HP)
{
	this->M_FCardActor_HP.M_Life = _HP;
}

bool ACardActor::GetMapDay()
{
	return this->M_Day;
}

bool ACardActor::GetCardDay()
{
	return this->M_CardDay;
}

void ACardActor::CardDayTransform(bool _Day)
{
	this->M_CardDay = _Day;
}

bool ACardActor::GetGameIsPause()
{
	return this->M_AGameMapInstance->M_VSManagerComponent->IsGamePause();;
}

AMouseActor* ACardActor::GetCurrentMouse()
{
	return this->M_CurrentMouse;
}

void ACardActor::SetCurrentMouse(AMouseActor* _MouseActor)
{
	this->M_CurrentMouse = _MouseActor;
}

int32 ACardActor::GetCardGrade(const FString& _Name)
{
	if (AGameMapInstance::GetGameMapInstance())
	{
		for (auto Datas : AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardData)
		{
			if (Datas.ItemName.Equals(_Name))
			{
				return Datas.M_CardGrade;
			}
		}
	}

	return int32();
}

FMouseLineNotify& ACardActor::GetMouseLineNotify()
{
	//UE_LOG(LogTemp, Warning, TEXT("%d  %d"), this->M_Line.Row, this->M_Line.Col);
	return this->M_AGameMapInstance->M_MouseManagerComponent->M_FMouseLineNotify[this->M_Line.Row];
}

bool ACardActor::UpdateCardState(const float& _ATK_Value, const float& _UpHP_Value)
{
	if (this->M_FCardActor_HP.M_Life <= 0.f)
	{
		return true;
	}

	if (_UpHP_Value + this->M_FCardActor_HP.M_Life >= this->M_FCardActor_HP.M_TotalLife)
	{
		this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_TotalLife;
	}
	else {
		this->M_FCardActor_HP.M_Life += _UpHP_Value;
	}

	this->M_FCardActor_HP.M_Life = this->M_FCardActor_HP.M_Life - _ATK_Value;

	return false;
}

void ACardActor::PrintMouseLineNotify()
{
	this->M_AGameMapInstance->M_MouseManagerComponent->PrintMouseLineNotify(this->GetMouseLineNotify());
}

void ACardActor::BeginPlay()
{
	Super::BeginPlay();

	this->M_AGameMapInstance = AGameMapInstance::GetGameMapInstance();
	if (!this->M_AGameMapInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[ACardActor::BeginPlay]:游戏地图实例对象获取失败"));
		this->SetActorTickEnabled(false);
		return;
	}

	this->SetActorRotation(FRotator(0.f, 90.f, 0.f));

	//获取白天和夜晚
	this->M_Day = this->M_AGameMapInstance->M_ResourceManagerComponent->GetDay();
}

void ACardActor::KillCard()
{
	if (this->M_UUI_MapMeshe)
	{
		this->M_UUI_MapMeshe->EradicateCard(this->M_CardID);

		//判断当前卡片是否开启了价格线性增长，如果开启了则降低价格->死亡一次降低一次价格
		//首先查询当前卡片是否在列表->如果在列表才会成立
		for (auto Datas = M_AGameMapInstance->M_CardManagerComponent->M_CardData.CreateIterator(); Datas; Datas++)
		{
			//如果查询到
			if ((*Datas).ItemName.Equals(this->M_CardName) && (*Datas).M_CardPriceAutoUp)
			{
				(*Datas).M_CardPrice -= (*Datas).M_CardPriceUpNum;

				if (this->M_UUI_Card)
					this->M_UUI_Card->M_NeedFlame = (*Datas).M_CardPrice;

				break;
			}
		}

		if (!this->IsPendingKillPending())
			this->Destroy();
	}
}

void ACardActor::SetUIMapMesh(UUI_MapMeshe* _MapMesh)
{
	this->M_UUI_MapMeshe = _MapMesh;
}

UUI_MapMeshe* ACardActor::GetUIMapMesh()
{
	return this->M_UUI_MapMeshe;
}

void ACardActor::AnalysisActor(AActor* _Actor)
{
	if (Cast<AMouseActor>(_Actor))
	{
		this->SetCurrentMouse(Cast<AMouseActor>(_Actor));
	}
}

ACardActor::ACardActor()
{
	PrimaryActorTick.bCanEverTick = true;

	this->M_CardGradeFlip = this->CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CardGrade"));
	this->M_CardGradeFlip->SetupAttachment(this->GetRenderComponent());
}

void ACardActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);



	//卡片休息->颜色设置
	if (!this->GetCardDay() && this->GetMapDay())
	{
		if (!this->M_bColorSet)
		{
			this->SetFlipbookColor(FVector(0.2f), 1.f);
			this->M_bColorSet = true;
		}
	}
	else {
		if (this->M_bColorSet)
		{
			this->SetFlipbookColor(FVector(1.f), 1.f);
			this->M_bColorSet = false;
		}
	}

}