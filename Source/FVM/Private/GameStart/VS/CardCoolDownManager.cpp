// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/CardCoolDownManager.h"

// Sets default values
ACardCoolDownManager::ACardCoolDownManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

float ACardCoolDownManager::GetFinishRate( )
{
	if (this->CoolDownTime > 0.f)
		return this->CurrentCoolDownTime /  this->CoolDownTime;

	return 0.f;
}

void ACardCoolDownManager::SetCoolDownFinish(bool _finish)
{
	this->IsCoolDownFinish = _finish;
}

bool ACardCoolDownManager::GetIsCoolDownFinish()
{
	return this->IsCoolDownFinish;
}

void ACardCoolDownManager::SetCoolDownEnable(bool _enable)
{
	this->SetActorTickEnabled(_enable);
}

void ACardCoolDownManager::SetCoolDownTime(const float& _cooldown_time)
{
	this->CoolDownTime = _cooldown_time;
}

// Called when the game starts or when spawned
void ACardCoolDownManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACardCoolDownManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!this->IsCoolDownFinish)
	{
		this->CurrentCoolDownTime += DeltaTime;
		if (this->CurrentCoolDownTime >= this->CoolDownTime)
		{
			this->CurrentCoolDownTime = 0.f;
			this->IsCoolDownFinish = true;
		}
	}
}

