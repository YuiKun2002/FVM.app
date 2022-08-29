// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MouseRound.h"

#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/VerticalBox.h>

bool UUI_MouseRound::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_USlider = this->GetWidgetComponent<USlider>(this, "MouseRoundSlider");
	this->M_MouseRoundText = this->GetWidgetComponent<UTextBlock>(this, "MouseRoundText");
	this->M_Boss_List_VerticalBox = this->GetWidgetComponent<UVerticalBox>(this, "Boss_HP_List");

	return true;
}

void UUI_MouseRound::UpdateMouseRoundSlider(const float& _Curr, const float& _End)
{
	this->M_USlider->SetValue(_Curr / _End);
}

void UUI_MouseRound::UpdateMouseRoundText(const int32& _Round)
{
	this->M_MouseRoundText->SetText(FText::FromString(FString::FormatAsNumber(_Round)));
}

void UUI_MouseRound::AddMouseBossSlider(UWidget* _Widget_)
{
	M_Boss_List_VerticalBox->AddChildToVerticalBox(_Widget_);
}
