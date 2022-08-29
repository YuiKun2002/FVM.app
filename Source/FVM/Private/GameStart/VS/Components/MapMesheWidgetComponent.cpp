// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MapMesheWidgetComponent.h"
#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include <Blueprint/UserWidget.h>

void UMapMesheWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	this->bReceiveHardwareInput = true;

	//加载ui
	UUI_MapMeshe* L_UUI_MapMeshe = CreateWidget<UUI_MapMeshe>(this->GetWorld(), LoadClass<UUI_MapMeshe>(nullptr, TEXT("WidgetBlueprint'/Game/Resource/BP/GameStart/VS/UI_MapMeshe.UI_MapMeshe_C'")));
	if (!L_UUI_MapMeshe)
	{
		UE_LOG(LogTemp, Error, TEXT("[UMapMesheInteractionComponent::OnComponentCreated]: UUI_MapMeshe 加载失败!"));
		return;
	}
	this->SetWidgetClass(L_UUI_MapMeshe->GetClass());
	this->SetDrawSize(FVector2D(50.f, 50.f));
}