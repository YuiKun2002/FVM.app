// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/MapMeshe.h"

#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/Components/MesheControllComponent.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameStart/VS/CreateCardBar.h"
#include "GameStart/VS/UI/UI_Card.h"
#include <Kismet/GameplayStatics.h>

#include <Components/Image.h>
void UUI_MapMeshe::PlayCard_PlayBGM()
{
	if (this->M_ELineType == ELineType::OnWater)
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("PlayCardToOnWater", "ItemAudio");
	}
	else {
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S("PlayCardToGround", "ItemAudio");
	}
}

void UUI_MapMeshe::PlayCard(UCardManagerComponent* _CardMComp, FItemCard& _CardData)
{

	ACardActor* Card = Cast <ACardActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, _CardMComp->GetCurrentSelectActor(), this->M_MapMesheTransform));

	//设置线路
	Card->M_Line = this->M_Line;
	//设置UIMapMesh
	Card->SetUIMapMesh(this);
	//设置UICard
	Card->M_UUI_Card = _CardMComp->GetUICard(_CardData.ItemTarget_ActorFileName);

	//产生阶段结束
	UGameplayStatics::FinishSpawningActor(Card, this->M_MapMesheTransform);


	//ACardActor* Card = this->GetWorld()->SpawnActor<ACardActor>(_CardMComp->GetCurrentSelectActor(), this->M_MapMesheTransform);



	if (_CardMComp->GetCurrentCoolDownManager() && !_CardMComp->GetCurrentCoolDownManager()->IsPendingKillPending())
	{
		//重置冷却
		_CardMComp->GetCurrentCoolDownManager()->SetCoolDownFinish(false);
	}

	//减去费用
	AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->SubtractFlameNum(_CardMComp->GetCardFlameNum(_CardMComp->M_CurrentSelectCardName));

	//判断卡片是否开启费用线性增加
	if (_CardData.M_CardPriceAutoUp)
	{
		_CardData.M_CardPrice += _CardData.M_CardPriceUpNum;
		UUI_Card* const UICard = _CardMComp->GetUICard(_CardData.ItemTarget_ActorFileName);
		UICard->M_NeedFlame = _CardData.M_CardPrice;
	}

	//添加层级
	this->IdAdd(_CardData.M_CardLayer, Card);

	//取消当前卡片的选择
	_CardMComp->CancelSelect();

	//播放音效
	this->PlayCard_PlayBGM();
}

const TMap<int32, ACardActor*>& UUI_MapMeshe::GetCardDatas()
{
	return this->M_Card_Data;
}

bool UUI_MapMeshe::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Gird = this->GetWidgetComponent<UImage>(this, "Meshe");

	//this->SetColor(FVector(1.f), 1.f);

	return true;
}

bool UUI_MapMeshe::IdAdd(const int32 _ID, ACardActor* _Card)
{
	if (!_Card)
		return false;

	ACardActor** _Reult = this->M_Card_Data.Find(_ID);

	if (!_Reult)
	{
		this->M_Card_Data.Emplace(_ID, _Card);
		return true;
	}

	return false;
}

bool UUI_MapMeshe::TestID(const int32 _ID)
{
	ACardActor** _Reult = this->M_Card_Data.Find(_ID);
	//寻找卡片ID如果ID唯一则返回true
	if (!_Reult)
	{
		return true;
	}
	return false;
}

void UUI_MapMeshe::EradicateCard()
{
	//销毁顺序
	TArray<int8> M_Card_Layer = { 5,4,3,2,0,1,-1 };

	for (auto PP = M_Card_Layer.CreateConstIterator(); PP; PP++)
	{
		if (this->EradicateCard((*PP)))
		{
			return;
		}
	}
}

void UUI_MapMeshe::SetColor(FVector _Vec, float _Alpha)
{
	UWidgetBase::SetImageColor(this->M_Gird, _Vec, _Alpha);
}

bool UUI_MapMeshe::EradicateCard(const int32& _CardLayer)
{

	//UE_LOG(LogTemp, Error, TEXT("%d"), _CardLayer);

	ACardActor** _Reult = this->M_Card_Data.Find(_CardLayer);

	if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending())
	{
		this->M_Card_Data.Remove(_CardLayer);
		(*_Reult)->Destroy();
		return true;
	}

	return false;
}

void UUI_MapMeshe::CreateNewCard(UCardManagerComponent* _CardMComp)
{
	//如果选择了铲卡
	if (_CardMComp->M_CurrentSelectEradicate)
	{
		this->EradicateCard();
		_CardMComp->CancelEradicate();
		AFlipbookActorBase* Eradicate_ = this->GetWorld()->SpawnActor<AFlipbookActorBase>(LoadClass<AFlipbookActorBase>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/BPEradicate.BPEradicate_C'")), this->M_MapMesheTransform);
		Eradicate_->IsPlayEndOnDestroy = true;
		UFVMGameInstance::PlayBGM_S_Static("PlayCardToGround", "ItemAudio");
		return;
	}

	//获取当前选择的卡片
	if (_CardMComp->GetCurrentSelectActor())
	{
		//获取卡片数据
		FItemCard& CardData = _CardMComp->GetCardData(_CardMComp->M_CurrentSelectCardName);

		//如果允许则直接放置
		if (CardData.M_ELineType == ELineType::All)
		{
			this->PlayCard(_CardMComp, CardData);
			_CardMComp->CancelSelect();
			return;
		}

		//判断是否适应地形（当卡片线路和需求线路类型不匹配）
		if (this->M_ELineType != CardData.M_ELineType)
		{
			//_CardMComp->CancelSelect();

			//判断当前地形中有没有卡片是-1承载类型的并且当前仿制的卡片不能是水上卡片
			if (CardData.M_ELineType != ELineType::OnWater)
			{
				//查询卡片 ->层级为-1
				ACardActor** _Reult = this->M_Card_Data.Find(-1);

				//如果查询成功并且这个卡片类型和当前网格类型匹配
				if (_Reult && (*_Reult) && !(*_Reult)->IsPendingKillPending() && (*_Reult)->M_ELineType == this->M_ELineType)
				{

					if (this->TestID(CardData.M_CardLayer))
					{
						this->PlayCard(_CardMComp, CardData);
					}

				}
			}

			_CardMComp->CancelSelect();
			return;
		}

		//判断卡片是否可以放置
		if (this->TestID(CardData.M_CardLayer))
		{
			this->PlayCard(_CardMComp, CardData);
		}
		else {
			//卡片不能放置->判断当前卡片是否可以重叠->如果可以重叠->那么替换卡片的实例重新设置数据
		}

	}

	/*
		else {
		//没有选择卡片创建3D -> UI
		if (_CardMComp->M_3DUIActor)
			_CardMComp->M_3DUIActor->Destroy();

		ACreateCardBar* _CreateCardBar = this->GetWorld()->SpawnActor<ACreateCardBar>(LoadClass<ACreateCardBar>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/BPCreateCardBar.BPCreateCardBar_C'")), this->M_MapMesheTransform);
		_CreateCardBar->M_UI_MapMeshe = this;
		_CreateCardBar->Init();

		_CardMComp->M_3DUIActor = _CreateCardBar;
	}

	*/

}

FVector UUI_MapMeshe::GetMapMeshCurrentTop(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentTop(_Col);
	return Meshe->GetWidgetComponentTransform().GetLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentBottom(int32 _Col)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentBottom(_Col);
	return Meshe->GetWidgetComponentTransform().GetLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentLeft(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentLeft(Row);
	return Meshe->GetWidgetComponentTransform().GetLocation();
}

FVector UUI_MapMeshe::GetMapMeshCurrentRight(int32 Row)
{
	AMapMeshe* Meshe = AGameMapInstance::GetGameMapInstance()->M_MesheControllComponent->GetMapMeshCurrentRight(Row);
	//FVector A = Meshe->GetWidgetComponentTransform().GetLocation();
	//UE_LOG(LogTemp, Error, TEXT("%d %.2f %.2f %.2f"), Row,A.X, A.Y, A.Z);
	return Meshe->GetWidgetComponentTransform().GetLocation();
}
