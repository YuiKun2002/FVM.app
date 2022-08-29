// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_BagRemoveItemBase.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include <Components/Image.h>
#include <Components/Button.h>

void UUI_BagRemoveItemBase::SelectItems(bool _Tag)
{
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	if (this->M_PanelNumber == 0) {
		PlayerData->M_PlayerItems_Equipment[M_ItemIndex].bWaitRemove = _Tag;
	}
	if (this->M_PanelNumber == 1) {
		PlayerData->M_PlayerItems_Card[M_ItemIndex].bWaitRemove = _Tag;
	}
	if (this->M_PanelNumber == 2) {
		PlayerData->M_PlayerItems_Material[M_ItemIndex].bWaitRemove = _Tag;
	}

	if (!_Tag)
	{
		this->M_bIsSelectItem = false;
		this->M_ShowSelectTag->SetVisibility(ESlateVisibility::Hidden);
		M_SelectButtonText = FString(TEXT("选择"));
	}

}

void UUI_BagRemoveItemBase::Select()
{
	if (this->M_bIsSelectItem)
	{
		this->M_bIsSelectItem = false;
		//取消选择
		this->SelectItems(false);
		this->M_ShowSelectTag->SetVisibility(ESlateVisibility::Hidden);
		M_SelectButtonText = FString(TEXT("选择"));
	}
	else {
		this->M_bIsSelectItem = true;
		//选择
		this->SelectItems(true);
		UWidgetBase::SetImageBrush(this->M_ShowSelectTag, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_13.T_PB_13'");
		M_SelectButtonText = FString(TEXT("取消选择"));
	}
}

bool UUI_BagRemoveItemBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_SelectButton = this->GetWidgetComponent<UButton>(this, "Select");
	this->M_ShowSelectTag = this->GetWidgetComponent<UImage>(this, "SelecState_Image");

	M_SelectButtonText = FString(TEXT("取消选择"));
	return true;
}

void UUI_BagRemoveItemBase::SetTagItems(int32 _ItemIndex, uint8 _PanelNumber)
{
	this->M_ItemIndex = _ItemIndex;
	this->M_PanelNumber = _PanelNumber;
}

UButton* UUI_BagRemoveItemBase::GetSelectButton()
{
	return this->M_SelectButton;
}

void UUI_BagRemoveItemBase::BindSelectFunc()
{
	FScriptDelegate Func;
	Func.BindUFunction(this, "Select");
	this->M_SelectButton->OnClicked.Add(Func);
}
