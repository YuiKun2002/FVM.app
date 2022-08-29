// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardFunctionComponent.generated.h"

class ACardActor;
class UUI_MapMeshe;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardFunctionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//¿¨Æ¬×é¼þ
	ACardActor* M_Owner = nullptr;
public:
	// Sets default values for this component's properties
	UCardFunctionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	void CardFunc_TransfromDayToNight();
	void CardFunc_TransfromNightToDay();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
