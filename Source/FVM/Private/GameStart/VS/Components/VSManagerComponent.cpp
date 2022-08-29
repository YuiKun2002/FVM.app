// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/VSManagerComponent.h"

#include "GameStart/VS/UI/UI_GameOver.h"

#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/FVMGameInstance.h"

// Sets default values for this component's properties
UVSManagerComponent::UVSManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVSManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UVSManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

bool UVSManagerComponent::IsGamePause()
{
	return this->M_bGamePause;
}

void UVSManagerComponent::GamePause()
{
	this->M_bGamePause = true;
}

void UVSManagerComponent::GameContinue()
{
	this->M_bGamePause = false;
}

void UVSManagerComponent::GameStart()
{

}

void UVSManagerComponent::GameOver()
{
	this->M_UI_GameOver = CreateWidget<UUI_GameOver>(this->GetWorld(), LoadClass<UUI_GameOver>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_Player/UI_GameOver.UI_GameOver_C'")));
	this->M_UI_GameOver->AddToViewport();

	//播放音乐
	if (M_Win)
	{ 
		UFVMGameInstance::PlayBGM_Static("");
		UFVMGameInstance::PlayBGM_S_Static("Win");
	}
	else {
		UFVMGameInstance::PlayBGM_Static("");
		UFVMGameInstance::PlayBGM_S_Static("Failed");
	}
}

