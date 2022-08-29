// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCruveComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UShootCruveComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	//目标的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FVector Target;

	//曲线最高高度
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float MaxHight = 5.f;
public:
	// Sets default values for this component's properties
	UShootCruveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//获取曲线路线
	UFUNCTION(BlueprintCallable)
		FVector GetCruveLine(float FlyTime,float FlyTime_MaxHight, FVector _CurrentLocation,FVector _AddTargetOffSet);
};
