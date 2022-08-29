// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardFunctionComponent.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
// Sets default values for this component's properties
UCardFunctionComponent::UCardFunctionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardFunctionComponent::BeginPlay()
{
	Super::BeginPlay();

	//获取卡片拥有者
	this->M_Owner = Cast<ACardActor>(this->GetOwner());
	if (!this->M_Owner)
	{
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardFunctionComponent::CardFunc_TransfromDayToNight()
{
	if (this->M_Owner->GetCardFunctionData().M_TransfromDayToNignt)
	{
		for (auto& Data : this->M_Owner->GetUIMapMesh()->GetCardDatas())
		{
			if (Data.Value->GetCardDay())
			{
				Data.Value->CardDayTransform(false);
			}
		}
	}
}

void UCardFunctionComponent::CardFunc_TransfromNightToDay()
{
	if (this->M_Owner->GetCardFunctionData().M_TransfromNigntToDay)
	{
		for (auto& Data : this->M_Owner->GetUIMapMesh()->GetCardDatas())
		{
			if (!Data.Value->GetCardDay())
			{
				Data.Value->CardDayTransform(true);
			}
		}
	}
}


// Called every frame
void UCardFunctionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//卡片休息
	if (!this->M_Owner->GetCardDay() && this->M_Owner->GetMapDay())
		return;

	if (this->M_Owner->AnimationIsEnd())
	{
		this->SetComponentTickEnabled(false);
		//调用功能
		this->CardFunc_TransfromDayToNight();
		this->CardFunc_TransfromNightToDay();
		//销毁卡片
		this->M_Owner->KillCard();
	}
}

