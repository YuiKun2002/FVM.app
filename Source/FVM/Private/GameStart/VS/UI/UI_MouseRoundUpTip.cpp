// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_MouseRoundUpTip.h"

#include "GameStart/VS/GameMapInstance.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"

#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/Components/VSManagerComponent.h"

#include "GameStart/VS/PlayerView.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

bool UUI_MouseRoundUpTip::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Time_Text = this->GetWidgetComponent<UTextBlock>(this, "Time_Text");
	this->M_Key_Name = this->GetWidgetComponent<UTextBlock>(this, "Key_Name");
	this->M_Key_Image = this->GetWidgetComponent<UImage>(this, "Key_Image");



	return true;
}

void UUI_MouseRoundUpTip::SetKeyImage(const FString& _ImageName)
{
	const FString& _ImagePath = FString("Texture2D'/Game/Resource/Texture/PlayerItems/Keys/") + _ImageName + FString(".") + _ImageName + FString("'");
	this->M_Key_Image->SetBrushFromTexture(LoadObject<UTexture2D>(0, *_ImagePath));
}

void UUI_MouseRoundUpTip::SetKeyName(const FString& _KeyName)
{
	this->M_Key_Name->SetText(FText::FromString(_KeyName));
}

void UUI_MouseRoundUpTip::GameWin()
{
	AGameMapInstance::GetGameMapInstance()->GameOver();
}

void UUI_MouseRoundUpTip::GameContinue()
{
	//²¥·ÅBGM
	AGameMapInstance::GetGameMapInstance()->PlaySound(AGameMapInstance::GetGameMapInstance()->GetGameMapStructManager()->M_Data.M_Begin_UpRound_Music);
	AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent->InitNextRound();

	APlayerView::GetPlayerViewInstance()->EnableTouch();
	AGameMapInstance::GetGameMapInstance()->M_VSManagerComponent->GameContinue();
}
