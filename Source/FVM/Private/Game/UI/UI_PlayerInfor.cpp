// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerInfor.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

#include <Components/Image.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

bool UUI_PlayerInfor::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_PlayerHead = this->GetWidgetComponent<UImage>(this, "PlayerHead");
	this->M_PlayerGrade = this->GetWidgetComponent<UImage>(this, "PlayerGrade");

	this->M_Player_Progress_Main = this->GetWidgetComponent<UImage>(this, "Slider_Main");
	this->M_Player_Progress_Bg = this->GetWidgetComponent<UImage>(this, "Slider_BG");
	this->M_Player_Progress_Text = this->GetWidgetComponent<UTextBlock>(this, "Slider_Text");

	this->M_Player_VIP = this->GetWidgetComponent<UButton>(this, "VIP");

	return true;
}



void UUI_PlayerInfor::UpdatePlayerHead()
{

}

void UUI_PlayerInfor::UpdatePlayerInfor()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	this->M_PlayerName = Player->M_PlayerName;
	FString _Tips[6] = { TEXT("新手冒险家"),TEXT("初级冒险家"),TEXT("中级冒险家"),TEXT("巨匠冒险家"),TEXT("超神秘冒险家"),TEXT("传说冒险家") };
	this->M_PlayerTip = _Tips[0];
	this->M_PlayerLog = TEXT("未来可自定义");

	//更新VIP显示
	UWidgetBase::SetButtonStyle(this->M_Player_VIP, FString("Texture2D'/Game/Resource/Texture/VIP/0/" + FString::FormatAsNumber(Player->M_FPlayerVIP.M_VIP + 1) + "." + FString::FormatAsNumber(Player->M_FPlayerVIP.M_VIP + 1) + "'"));
}

void UUI_PlayerInfor::UpdatePlayerGrade()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();
	FString _Grade_Path = "Texture2D'/Game/Resource/Texture/PlayerGrade/" + FString::FormatAsNumber(Player->M_FPlayerEx.M_PlayerGrade) + "." + FString::FormatAsNumber(Player->M_FPlayerEx.M_PlayerGrade) + "'";
	UWidgetBase::SetImageBrush(this->M_PlayerGrade, _Grade_Path);
}

void UUI_PlayerInfor::UpdatePlayerEx()
{
	UPlayerStructManager* Player = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	float _Value = UKismetMathLibrary::Lerp(3.5f, 322.f, Player->M_FPlayerEx.M_CurrentEx / Player->M_FPlayerEx.M_CurrentExTop);

	if (Cast<UCanvasPanel>(this->M_Player_Progress_Main))
	{
		Cast<UCanvasPanelSlot>(this->M_Player_Progress_Main)->SetSize(FVector2D(_Value, 41.25f));
		Cast<UCanvasPanelSlot>(this->M_Player_Progress_Main)->SetPosition(FVector2D(258.f, 37.5f));

		FText M_Content = FText::FromString(FString(FString::FormatAsNumber(_Value) + "%"));
		M_Player_Progress_Text->SetText(M_Content);
	}
}
