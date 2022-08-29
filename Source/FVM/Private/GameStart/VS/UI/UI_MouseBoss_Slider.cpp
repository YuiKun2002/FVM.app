// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MouseBoss_Slider.h"

#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

bool UUI_MouseBoss_Slider::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_BossCanvasPanel = this->GetWidgetComponent<UCanvasPanel>(this, "Boss");
	this->M_BossNameText = this->GetWidgetComponent<UTextBlock>(this, "BossName");
	this->M_BossHead = this->GetWidgetComponent<UImage>(this, "BossHead");
	this->M_Slider_Image = this->GetWidgetComponent<UImage>(this, "Slider_Pro_Boos_0");

	return true;
}

void UUI_MouseBoss_Slider::SetSliderRate(float Value)
{
	// PX [-312 , 2.9]
	// SX [2.4  , 633]
	Cast<UCanvasPanelSlot>(M_Slider_Image->Slot)->SetPosition(FVector2D(-312.f + Value * (2.9f - (-312.f)), 0.9f));
	Cast<UCanvasPanelSlot>(M_Slider_Image->Slot)->SetSize(FVector2D(2.4f + Value * (633.f - 2.4f), 21.f));
}

void UUI_MouseBoss_Slider::SetBossName(const FString& _Name)
{
	this->M_BossNameText->SetText(FText::FromString(*_Name));
}

void UUI_MouseBoss_Slider::SetBossHead(const FString& _HeadName)
{
	const FString& _BossHeadPath = FString("Texture2D'/Game/Resource/Texture/Sprite/VS/Sprite/Mouse/BossHead/") + _HeadName + FString(".") + _HeadName;
	this->M_BossHead->SetBrushFromTexture(LoadObject<UTexture2D>(0, *_BossHeadPath));
}
