// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagEquipmentGrid.h"

#include "Game/UI/UI_PlayerBag.h"
#include "Game/UI/UI_GiftBox.h"

#include <Components/Button.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"


bool UUI_PlayerBagEquipmentGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UButton = this->GetWidgetComponent<UButton>(this, "Item_button");

	return true;
}

void UUI_PlayerBagEquipmentGrid::RelaodBagEquipment()
{
	if (this->M_BagUI)
	{
		this->M_BagUI->LoadItemsData();
	}
}

void UUI_PlayerBagEquipmentGrid::AttachToBag()
{
	if (this->M_BagUI)
	{
		FPlayerBagGirdGroup& Group = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

		switch (this->M_BagUI->GetPanel_Index())
		{
		case 0:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_EquipmentGrid, M_EuipmentData); break;
		case 1:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_CardGrid, M_EuipmentData); break;
		case 2:this->M_BagUI->AddBagGrid(this->M_BagUI->GetPanel_Index(), this->M_BagUI->M_BagNumber, Group.M_MaterialGrid, M_EuipmentData); break;
		}

		this->M_BagUI->CloseSelectBagPanel();

		//±£´æÊý¾Ý
		UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
	}
}

void UUI_PlayerBagEquipmentGrid::CreateNewGiftBox()
{
	UUI_GiftBox* GiftBox = CreateWidget<UUI_GiftBox>(this, LoadClass<UUI_GiftBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_GiftBox_Panel.BPUI_GiftBox_Panel_C'")));
	GiftBox->SetPlayerBagEquipmentGrid(this);
	GiftBox->SetGiftBoxData(this->GetItemName());
	GiftBox->AddToViewport();
}

void UUI_PlayerBagEquipmentGrid::SetItemName(const FString& _Name)
{
	this->M_ItemName = _Name;
}

FString UUI_PlayerBagEquipmentGrid::GetItemName()
{
	return this->M_ItemName;
}

UButton* UUI_PlayerBagEquipmentGrid::GetButton()
{
	return this->M_UButton;
}
