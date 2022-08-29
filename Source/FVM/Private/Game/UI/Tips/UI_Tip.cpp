// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Tips/UI_Tip.h"
#include <Components/TextBlock.h>

void UUI_Tip::SetTipText(const FString& _Text)
{
	this->M_ContentTips = _Text;
}

void UUI_Tip::SetTextColor(const FVector& _Vec3, float Alpha)
{
	if (!this->M_Text)
		return;
	this->M_Text->SetColorAndOpacity(FSlateColor(FLinearColor(_Vec3.X, _Vec3.Y, _Vec3.Z, Alpha)));
}

bool UUI_Tip::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Text = this->GetWidgetComponent<UTextBlock>(this, "TextBlock_31");

	return true;
}