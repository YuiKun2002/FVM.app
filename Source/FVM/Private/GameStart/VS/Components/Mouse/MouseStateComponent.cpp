// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Mouse/MouseStateComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

// Sets default values for this component's properties
UMouseStateComponent::UMouseStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMouseStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UMouseStateComponent::InitResource()
{
	this->M_UPaperFlipbook_Def = LoadObject<UPaperFlipbook>(nullptr, *this->M_MouseDef);
	if (!this->M_UPaperFlipbook_Def)
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_UPaperFlipbook_Def动画加载失败"));

	this->M_UPaperFlipbook_Attack = LoadObject<UPaperFlipbook>(nullptr, *this->M_MouseAttack);
	if (!this->M_UPaperFlipbook_Attack)
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_UPaperFlipbook_Attack动画加载失败"));

	this->M_UPaperFlipbook_LowDef = LoadObject<UPaperFlipbook>(nullptr, *this->M_MouseLowDef);
	if (!this->M_UPaperFlipbook_LowDef)
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_UPaperFlipbook_LowDef动画加载失败"));

	this->M_UPaperFlipbook_LowAttack = LoadObject<UPaperFlipbook>(nullptr, *this->M_MouseLowAttack);
	if (!this->M_UPaperFlipbook_LowAttack)
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_UPaperFlipbook_LowAttack动画加载失败"));

	this->M_UPaperFlipbook_Death = LoadObject<UPaperFlipbook>(nullptr, *this->M_MouseDeath);
	if (!this->M_UPaperFlipbook_Death)
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_UPaperFlipbook_Death动画加载失败"));

	//获取主角
	this->M_Owner = Cast<AMouseActor>(this->GetOwner());
	if (!this->M_Owner)
	{
		this->SetComponentTickEnabled(false);
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateComponent::InitResource]:M_Owner获取失败"));
	}
	else {
		this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Def);
	}
}

bool UMouseStateComponent::UpdateState(AMouseActor* _MAMouseActor)
{
	//如果生命值为0  其他的任何状态都不用操作了
	if (_MAMouseActor->GetCurrentHP() <= 0)
	{
		_MAMouseActor->IsPlayEndOnDestroy = true;
		_MAMouseActor->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Death);
		return true;
	}

	//老鼠在攻击
	if (_MAMouseActor->GetbIsAttack())
	{
		if ((_MAMouseActor->GetTotalHP() / 2) >= _MAMouseActor->GetCurrentHP())
		{
			_MAMouseActor->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_LowAttack);
		}
		else {
			_MAMouseActor->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Attack);
		}
	}
	else {
		//如果生命过半
		if ((_MAMouseActor->GetTotalHP() / 2) >= _MAMouseActor->GetCurrentHP())
		{
			_MAMouseActor->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_LowDef);
		}
		else {
			_MAMouseActor->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Def);
		}
	}

	return false;
}

// Called every frame
void UMouseStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
