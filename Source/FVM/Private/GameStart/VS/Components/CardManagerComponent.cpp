// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/CardManagerComponent.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/UI/UI_Card.h"

#include <Kismet/KismetMathLibrary.h>

#include "GameStart/VS/UI/UI_CardBar.h"
#include <Components/HorizontalBox.h>
#include <Components/Image.h>
#include <Engine/Texture2D.h>

// Sets default values for this component's properties
UCardManagerComponent::UCardManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardManagerComponent::BeginPlay()
{
	Super::BeginPlay();


	// ...

}


TSubclassOf<ACardActor>& UCardManagerComponent::GetCurrentSelectActor()
{
	return this->M_CurrentSelectCardInstance;
}

ACardCoolDownManager* UCardManagerComponent::GetCurrentCoolDownManager()
{
	if (this->M_CardName.Find(this->M_CurrentSelectCardName) == -1)
		return nullptr;

	return this->M_CardCoolDownManager[this->M_CardName.Find(this->M_CurrentSelectCardName)];
}

void UCardManagerComponent::SetCard_Image(UUI_Card* _CardUI, const FString& _CardBg)
{
	if (_CardUI)
	{
		//对应卡片的背景路径
		_CardUI->M_UImage->SetBrushFromTexture(LoadObject<UTexture2D>(0, *_CardBg));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("卡片背景图片设置失败，因为当前卡片UI无效"));
	}
}

bool UCardManagerComponent::SelectCurrentActor(const FString& _CardName)
{
	//如果当前选择的卡片名称和之前选择的卡片名称一样
	if (this->M_CurrentSelectCardName.Equals(_CardName))
	{
		//则取消选择
		this->CancelSelect();

		//卡片名称相同
		UE_LOG(LogTemp, Error, TEXT("选择的卡片名称相同"));

		return false;
	}

	//则取消选择
	this->CancelSelect();

	//取消对铲子的选择
	this->CancelEradicate();

	//播放BGM——S
	UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("TakeCard", "ItemAudio");

	//设置选择的名称
	this->M_CurrentSelectCardName = _CardName;

	//获取UICard
	if (this->GetUICard(this->M_CurrentSelectCardName))
	{
		this->GetUICard(_CardName)->SelectColor();
	}

	// Blueprint'/Game/Resource/BP/GameStart/Item/Card/Card_1.Card_1'

	//加载Actor资源
	FString Ref = FString("Blueprint'/Game/Resource/BP/GameStart/Item/Card/" + _CardName + "." + _CardName + "_C'");

	this->M_CurrentSelectCardInstance = LoadClass<ACardActor>(0, *Ref);

	if (!this->M_CurrentSelectCardInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[UCardManagerComponent::SelectCurrentActor]:卡片选择失败,因为加载的对象不存在!"));
		return false;
	}

	return true;
}

const int32 UCardManagerComponent::GetCardFlameNum(const FString& _CardActorName)
{
	for (auto Datas : this->M_CardData)
	{
		if (Datas.ItemTarget_ActorFileName.Equals(_CardActorName))
			return Datas.M_CardPrice;
	}
	return 0;
}

FItemCard& UCardManagerComponent::GetCardData(const FString& _CardActorName)
{
	static FItemCard _StaticTemp;

	for (auto PP = this->M_CardData.CreateConstIterator(); PP; PP++)
	{
		if (PP->ItemTarget_ActorFileName.Equals(_CardActorName))
		{
			return this->M_CardData[PP.GetIndex()];
		}
	}

	return _StaticTemp;
}

UUI_Card* const UCardManagerComponent::GetUICard(const FString& _CardActorName)
{
	for (auto Datas : this->M_UICard)
	{
		if (Datas->M_Card_ActorName.Equals(_CardActorName))
			return Datas;
	}
	return nullptr;
}

const FString& UCardManagerComponent::GetCurrentSelectCardName()
{
	return this->M_CurrentSelectCardName;
}

void UCardManagerComponent::CancelSelect()
{
	//获取UICard
	if (this->GetUICard(this->M_CurrentSelectCardName))
	{
		this->GetUICard(this->M_CurrentSelectCardName)->CancelColor();
	}

	if (this->M_CurrentSelectCardInstance)
		this->M_CurrentSelectCardInstance = nullptr;

	//清空名称
	this->M_CurrentSelectCardName.Empty();
}

void UCardManagerComponent::SelectEradicate()
{
	this->M_CurrentSelectEradicate = true;
	this->CancelSelect();

	UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("TakeEradicate", "ItemAudio");
}

void UCardManagerComponent::CancelEradicate()
{
	this->M_CurrentSelectEradicate = false;
}

void UCardManagerComponent::AddCard(const FItemCard& _CardData)
{
	this->M_CardName.Emplace(_CardData.ItemTarget_ActorFileName);

	if (this->M_UUI_CardBar)
	{
		UUI_Card* CardBoxWidget = CreateWidget<UUI_Card>(this->GetWorld(), LoadClass<UUI_Card>(nullptr, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UICardBad/CardBoxHorizental.CardBoxHorizental_C'")));
		CardBoxWidget->M_CardIndex = this->M_CardName.Num() - 1;
		CardBoxWidget->M_NeedFlame = _CardData.M_CardPrice;
		CardBoxWidget->M_Card_ActorName = _CardData.ItemTarget_ActorFileName;//该名称不是卡片实例的名称是文件名称
		CardBoxWidget->SetCardGrade(_CardData.M_CardGrade);
		this->SetCard_Image(CardBoxWidget, _CardData.ItemTexturePath);

		//当前冷却
		float _CardClodDown = 0.f;

		UE_LOG(LogTemp, Error, TEXT("%d"), (uint8)(_CardData.M_ECardUpGradeType));

		//计算冷却
		if (_CardData.M_ECardUpGradeType == ECardUpGradeType::E_ColdDownSpeed_Up)
			_CardClodDown = _CardData.M_CardColdDown - (_CardData.M_CardColdDown * _CardData.M_CardGrade * (_CardData.M_M_ECardUpGradeUpRate / 100.f));
		else
			_CardClodDown = _CardData.M_CardColdDown;

		//设置冷却
		this->AddCoolDown(_CardClodDown);

		this->M_UUI_CardBar->GetCardList()->AddChildToHorizontalBox(CardBoxWidget);
		this->M_UICard.Emplace(CardBoxWidget);
	}
}

bool UCardManagerComponent::AddSpecialCard(const int32& _CardListIndex)
{
	if (this->M_SpecialCardListIndex.Num() < 4)
	{
		this->M_SpecialCardListIndex.Emplace(_CardListIndex);

		return true;
	}

	//UKismetMathLibrary::Lerp();

	return false;
}

void UCardManagerComponent::AddCoolDown(const float& _Time)
{
	ACardCoolDownManager* M_ACardCoolDownManager = this->GetWorld()->SpawnActor<ACardCoolDownManager>();
	M_ACardCoolDownManager->SetCoolDownTime(_Time);
	this->M_CardCoolDownManager.Emplace(M_ACardCoolDownManager);
}

void UCardManagerComponent::HiddenCardBar()
{
	if (this->M_UUI_CardBar->IsInViewport())
		this->M_UUI_CardBar->RemoveFromParent();
}

void UCardManagerComponent::ShowCardBar()
{
	if (!this->M_UUI_CardBar->IsInViewport() && AGameMapInstance::GetGameMapInstance()->IsInGame)
		this->M_UUI_CardBar->AddToViewport();
}

bool UCardManagerComponent::GetCardCoolDownFinish(const int32& _Index)
{
	//获取卡片冷却管理器
	return this->M_CardCoolDownManager[_Index]->GetIsCoolDownFinish();
}

// Called every frame
void UCardManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCardManagerComponent::GameStart()
{
	//加载卡片栏UI
	if (!this->M_UUI_CardBar)
	{
		this->M_UUI_CardBar = CreateWidget<UUI_CardBar>(this->GetWorld(), LoadClass<UUI_CardBar>(nullptr, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UICardBar.UICardBar_C'")));
		if (!this->M_UUI_CardBar)
		{
			UE_LOG(LogTemp, Error, TEXT("卡片栏界面加载失败，路径有误!"));
			return;
		}
		this->M_UUI_CardBar->AddToViewport();
	}

	//初始化卡片
//	int32 Special = 0;
	for (auto CardDatas : UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerGamePrepareSelectCardData)
	{
		this->AddCard(CardDatas);
		//this->AddSpecialCard(Special);
		this->M_CardData.Emplace(CardDatas);

		//UE_LOG(LogTemp, Error, TEXT("%d"), CardDatas.M_CardLayer);
		//	Special++;
	}

	//添加铲子
	UUserWidget* EradicateWidget = CreateWidget<UUserWidget>(this->GetWorld(), LoadClass<UUserWidget>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UICardBad/BPUI_EradicateCard.BPUI_EradicateCard_C'")));
	this->M_UUI_CardBar->GetCardList()->AddChildToHorizontalBox(EradicateWidget);
}

void UCardManagerComponent::GameOver()
{
	this->HiddenCardBar();

	this->M_CardName.Empty();
	this->M_SpecialCardListIndex.Empty();

	//移除卡片冷却管理器
	for (auto C_Pt = this->M_CardCoolDownManager.CreateIterator(); C_Pt; C_Pt++)
	{
		(*C_Pt)->Destroy();
	}

	//清空卡片冷却管理器
	this->M_CardCoolDownManager.Empty();

	//清空卡片ui
	this->M_UICard.Empty();
}

