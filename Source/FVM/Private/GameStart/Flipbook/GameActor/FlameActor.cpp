// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/FlameActor.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"

AFlameActor::AFlameActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFlameActor::SetFlameTime(const float& _time)
{
	//设置极限
	if (_time < 2.f || _time > 60.f)
		this->M_LifeTime = 10.f;

	this->M_LifeTime = _time;
}

void AFlameActor::SetFlameValue(const int32& _value)
{
	//设置极限
	if (_value < 15 || _value > 500)
		this->M_Value = 25;

	this->M_Value = _value;
}

void AFlameActor::BeginPlay()
{
	Super::BeginPlay();
}

void AFlameActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	this->M_CurrentTime += DeltaSeconds;
	if (this->M_CurrentTime >= this->M_LifeTime)
	{
		this->M_CurrentTime = 0.f;
		AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->AddFlameNum(this->M_Value);
		this->Destroy();
	}
}