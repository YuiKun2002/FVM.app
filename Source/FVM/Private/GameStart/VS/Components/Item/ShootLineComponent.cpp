// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Item/ShootLineComponent.h"
#include "GameStart/Flipbook/FlipbookActorBase.h"

// Sets default values for this component's properties
UShootLineComponent::UShootLineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UShootLineComponent::Hit(const ELineType& _EMouseType)
{
	if (this->M_NodePosition < this->M_MoveNode.Num())
	{
		for (const auto& Item : this->M_MoveNode[this->M_NodePosition].M_AttackType)
		{
			if (Item == _EMouseType)
				return true;
		}
	}
	return false;
}

void UShootLineComponent::Over()
{
	this->SetComponentTickEnabled(false);
}

void UShootLineComponent::Move(const float& X, const float& Y, const float& Z)
{
	//如果当前位置大于等于目标点
	if (this->M_MoveNode[this->M_NodePosition].M_Target <= this->M_Target)
	{
		//完成目标
		this->M_NodePosition++;
		this->M_Target = 0.f;
	}
	else {
		this->M_Target += this->M_MoveNode[this->M_NodePosition].M_Speed;
		this->M_Owner->AddActorLocalOffset(FVector(X, Y, Z));
	}
}

// Called when the game starts
void UShootLineComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_Owner = Cast<AFlipbookActorBase>(this->GetOwner());
	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("[UShootLineComponent::BeginPlay]: 拥有者不是一个Actor"));
		this->SetComponentTickEnabled(false);
		return;
	}

}


// Called every frame
void UShootLineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->M_NodePosition < this->M_MoveNode.Num())
	{
		//this->M_Owner->AddActorLocalOffset();
		switch (this->M_MoveNode[this->M_NodePosition].M_EShootDirection)
		{
		case EShootDirection::_EUp:
			this->M_Owner->SetActorRotation(FRotator(-90.f, 90.f, 0.f));
			this->Move(-this->M_MoveNode[this->M_NodePosition].M_Speed, 0.f, 0.f);
			break;

		case EShootDirection::_EDown:
			this->M_Owner->SetActorRotation(FRotator(90.f, 90.f, 0.f));
			this->Move(-this->M_MoveNode[this->M_NodePosition].M_Speed, 0.f, 0.f);
			break;

		case EShootDirection::_ELeft:
			this->M_Owner->SetActorRotation(FRotator(180.f, 90.f, 0.f));
			this->Move(-this->M_MoveNode[this->M_NodePosition].M_Speed, 0.f, 0.f);

			break;
		case EShootDirection::_ERight:
			this->M_Owner->SetActorRotation(FRotator(0.f, 90.f, 0.f));
			this->Move(this->M_MoveNode[this->M_NodePosition].M_Speed, 0.f, 0.f);
		}
	}
	else {
		//所有任务完成后销毁
		this->M_Owner->Destroy();

	}
}

