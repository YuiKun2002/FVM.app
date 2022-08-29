// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardDataComponent.h"

// Sets default values for this component's properties
UCardDataComponent::UCardDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


FItemCardATK UCardDataComponent::GetCardATKData(const FString& _CardName, bool _ToBag, int32 _CardGrade)
{
	return GetCardData<FItemCardATK, FCard_ATK_Data>(_CardName, ECardType::E_ATK, _ToBag, _CardGrade);
}

FItemCardSpawn UCardDataComponent::GetCardSpawnData(const FString& _CardName, bool _ToBag, int32 _CardGrade)
{
	return GetCardData<FItemCardSpawn, FCard_Spawn_Data>(_CardName, ECardType::E_SPAWN, _ToBag, _CardGrade);
}

FItemCardDefence UCardDataComponent::GetCardDefenceData(const FString& _CardName, bool _ToBag, int32 _CardGrade)
{
	return  GetCardData<FItemCardDefence, FCard_Defence_Data>(_CardName, ECardType::E_DEFENCE, _ToBag, _CardGrade);
}

FItemCardFunction UCardDataComponent::GetCardFunction(const FString& _CardName, bool _ToBag, int32 _CardGrade)
{
	return  GetCardData<FItemCardFunction, FCard_Function_Data>(_CardName, ECardType::E_Function, _ToBag, _CardGrade);
}

// Called when the game starts
void UCardDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UCardDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

