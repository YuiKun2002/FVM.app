// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardDefenceComponent.h"
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include <Kismet/GameplayStatics.h>
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

// Sets default values for this component's properties
UCardDefenceComponent::UCardDefenceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardDefenceComponent::UpdateState()
{
	if (this->M_Owner)
	{
		//获取到卡片生命值
		const float& HPTop = this->M_Owner->GetTotalHP();
		float HP = this->M_Owner->GetCurrentHP();

		////当前有老鼠时更新

		if (HP >= HPTop * (0.6f))
		{
			//第一状态
			this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);
		}
		else if (HP >= HPTop * (0.3f) && HP < HPTop * (0.6f))
		{
			//第二状态
			this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle2);
		}
		else {
			//第三状态
			this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle3);
		}
	}
}

void UCardDefenceComponent::LoadResource()
{
	this->M_UPaperFlipbook_Idle = LoadObject<UPaperFlipbook>(nullptr, *M_AnimIdle);
	this->M_UPaperFlipbook_Idle2 = LoadObject<UPaperFlipbook>(nullptr, *M_AnimIdle2);
	this->M_UPaperFlipbook_Idle3 = LoadObject<UPaperFlipbook>(nullptr, *M_AnimIdle3);
}


// Called when the game starts
void UCardDefenceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->M_Owner = Cast<ACardActor>(this->GetOwner());

	if (!this->M_Owner)
	{
		//UE_LOG(LogTemp, Error, TEXT("获取卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}


// Called every frame
void UCardDefenceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
		//卡片休息
	if (!this->M_Owner->GetCardDay() && this->M_Owner->GetMapDay())
		return;


}

