// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/Flipbook/GameActor/FlameActor.h"


// Sets default values for this component's properties
UResourceManagerComponent::UResourceManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResourceManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UResourceManagerComponent::EnableAutoFlameSpawn(const float& _SpawnTime)
{
	//判断计时器是否开启
	if (this->GetWorld()->GetTimerManager().IsTimerActive(this->M_TimerHandleAutoSpawnFlame))
	{
		this->GetWorld()->GetTimerManager().ClearTimer(this->M_TimerHandleAutoSpawnFlame);
	}
	else {
		//如果是白天
		if (this->IsDayTime)
		{
			this->GetWorld()->GetTimerManager().SetTimer(this->M_TimerHandleAutoSpawnFlame, [&]() {
				AFlameActor* L_AFlameActor = this->GetWorld()->SpawnActor<AFlameActor>(LoadClass<AFlameActor>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/Item/Resource/MyFlameActor.MyFlameActor_C'")));
				L_AFlameActor->SetActorLocation(FVector(540.f, -320.f, 0.f));
				L_AFlameActor->SetActorRotation(FRotator(0.f, 90.f, 0.f));
				}, _SpawnTime, true);
		}
	}
}

void UResourceManagerComponent::AddFlameNum(const int32& _Value)
{
	if (this->M_FlameNum + _Value > 1000000)
		this->M_FlameNum = 1000000;
	else
		this->M_FlameNum += _Value;
}

void UResourceManagerComponent::SubtractFlameNum(const int32& _Value)
{
	if (this->M_FlameNum - _Value < 0)
		this->M_FlameNum = 0;
	else
		this->M_FlameNum -= _Value;
}

const int32& UResourceManagerComponent::GetCurrentFlameNum()
{
	return this->M_FlameNum;
}


bool UResourceManagerComponent::GetDay()
{
	return this->IsDayTime;
}

// Called every frame
void UResourceManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResourceManagerComponent::GameStart()
{
	this->EnableAutoFlameSpawn(10.f);
}

void UResourceManagerComponent::GameOver()
{
	this->EnableAutoFlameSpawn(0.f);
}

