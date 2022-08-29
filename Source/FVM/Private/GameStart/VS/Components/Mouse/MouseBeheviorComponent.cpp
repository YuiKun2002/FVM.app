// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Mouse/MouseBeheviorComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameSystem/FVMGameInstance.h"

// Sets default values for this component's properties
UMouseBeheviorComponent::UMouseBeheviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMouseBeheviorComponent::BeginPlay()
{
	Super::BeginPlay();

	this->_Owner = Cast<AMouseActor>(this->GetOwner());
	if (!this->_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("[UMouseBeheviorComponent::BeginPlay]:当前组件的拥有者不是一个MouseActor"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UMouseBeheviorComponent::MoveToUp(const float& _DeltaTime)
{
	this->_Owner->AddActorLocalOffset(FVector(0.f, 0.f, _DeltaTime * this->_Owner->GetCurrentMoveSpeed() * 1.f * UFVMGameInstance::GetGameVSTimes()));
}

void UMouseBeheviorComponent::MoveToBottom(const float& _DeltaTime)
{
	this->_Owner->AddActorLocalOffset(FVector(0.f, 0.f, -(_DeltaTime * this->_Owner->GetCurrentMoveSpeed() * 1.f * UFVMGameInstance::GetGameVSTimes())));
}

void UMouseBeheviorComponent::MoveToLeft(const float& _DeltaTime)
{
	if (this->_Owner->GetActorScale3D().X < 0)
		this->_Owner->SetActorScale3D(FVector((this->_Owner->GetActorScale3D().X * -1), this->_Owner->GetActorScale3D().Y, this->_Owner->GetActorScale3D().Z));

	this->_Owner->AddActorLocalOffset(FVector(-(_DeltaTime * this->_Owner->GetCurrentMoveSpeed() * 1.f * UFVMGameInstance::GetGameVSTimes()), 0.f, 0.f));
}

void UMouseBeheviorComponent::MoveToRight(const float& _DeltaTime)
{
	if (this->_Owner->GetActorScale3D().X > 0)
		this->_Owner->SetActorScale3D(FVector((this->_Owner->GetActorScale3D().X * -1), this->_Owner->GetActorScale3D().Y, this->_Owner->GetActorScale3D().Z));

	this->_Owner->AddActorLocalOffset(FVector(_DeltaTime * this->_Owner->GetCurrentMoveSpeed() * 1.f * UFVMGameInstance::GetGameVSTimes(), 0.f, 0.f));
}



// Called every frame
void UMouseBeheviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//如果条件允许则可以移动
	if (this->_Owner->GetbIsMove())
		switch (this->_Owner->GetMouseMoveDirection())
		{
		case EMouseMoveDirection::_ETop:
			this->MoveToUp(DeltaTime);
			break;
		case EMouseMoveDirection::_EBottom:
			this->MoveToBottom(DeltaTime);
			break;
		case EMouseMoveDirection::_ELeft:
			this->MoveToLeft(DeltaTime);
			break;
		case EMouseMoveDirection::_ERight:
			this->MoveToRight(DeltaTime);
			break;
		}
}

