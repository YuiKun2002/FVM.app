// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/CreateCardBar.h"

#include <Components/WidgetComponent.h>

#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/UI_CardCommand.h"
#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/CardManagerComponent.h"

// Sets default values
ACreateCardBar::ACreateCardBar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_UWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComp_");
	this->SetRootComponent(this->M_UWidgetComponent);
}

void ACreateCardBar::Init()
{
	//如果网格有效
	if (this->M_UI_MapMeshe)
	{
		UUI_CardCommand* Widget_ = CreateWidget<UUI_CardCommand>(this->GetWorld(), LoadClass<UUI_CardCommand>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_CardCmd.UI_CardCmd_C'")));
		Widget_->M_ACreateCardBar = this;
		Widget_->Init();

		M_UWidgetComponent->SetWidget(Widget_);
		M_UWidgetComponent->SetDrawSize(FVector2D(298.f, 185.f));

		AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->HiddenCardBar();
	}
}

// Called when the game starts or when spawned
void ACreateCardBar::BeginPlay()
{
	Super::BeginPlay();

	this->AddActorLocalOffset(FVector(10.f, 0.f, 0.f));
	this->SetActorScale3D(FVector(0.9f));
}

// Called every frame
void ACreateCardBar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

