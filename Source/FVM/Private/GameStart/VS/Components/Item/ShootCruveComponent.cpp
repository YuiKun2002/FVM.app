// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/ShootCruveComponent.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UShootCruveComponent::UShootCruveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShootCruveComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UShootCruveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UShootCruveComponent::GetCruveLine(float FlyTime, float FlyTime_MaxHight, FVector _CurrentLocation, FVector _AddTargetOffSet)
{
	FVector A;
	A.X = _CurrentLocation.X;
	A.Y = _CurrentLocation.Y;
	A.Z = FlyTime_MaxHight * this->MaxHight + _CurrentLocation.Z;

	FVector B;
	B.X = this->Target.X + _AddTargetOffSet.X;
	B.Y = this->Target.Y + _AddTargetOffSet.Y;
	B.Z = FlyTime_MaxHight * this->MaxHight + (this->Target.Z + _AddTargetOffSet.Z);

	return UKismetMathLibrary::VLerp(A, B, FlyTime);
}

