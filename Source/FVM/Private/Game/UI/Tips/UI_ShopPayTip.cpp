// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Tips/UI_ShopPayTip.h"

#include <Components/Button.h>
#include <Components/EditableTextBox.h>
#include <Components/TextBlock.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerStructManager.h"

void UUI_ShopPayTip::PayButton()
{
	FString PlayerName = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName;
	if (UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(PlayerName))
	{
		UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

		FString BuyCount = this->M_PlayerPayCountTextBox->GetText().ToString();
		int32 ItemCount = FCString::Atoi(*BuyCount);

		//如果不是【背包道具】
		if (this->M_FItemPrice.M_ItemType != EItemType::E_BagGrid && ItemCount == 0)
			ItemCount = 1;
		else if (ItemCount > 999)
			ItemCount = 999;

		//购买
		if (FPlayerCoinAdd::Buy(PlayerData, this->M_FItemPrice.M_ItemPrice * ItemCount, this->M_FItemPrice.M_ItemMoneyType))
		{
			//根据类型支付
			switch (this->M_FItemPrice.M_ItemType)
			{
				//将玩家背包的第ItemCount界面的背包开启
			case EItemType::E_BagGrid:UShopItemPriceStruct::AddPlayerBagGrid(PlayerData, this->M_FItemPrice, ItemCount); break;
				//将物品添加到背包
			default:UShopItemPriceStruct::AddToPlayerBag(PlayerData, this->M_FItemPrice, ItemCount); break;
			}
			//保存数据
			UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(PlayerName);
			//移除界面
			this->RemoveFromParent();
			//购买成功
			FString Tip = TEXT("获得道具:") + this->M_FItemPrice.M_ItemName;
			UWidgetBase::CreateTipWidget(Tip, FVector(1.f, 0.5f, 0.f));
		}
		else
		{
			//货币不足
			FString Tip = TEXT("哎呀,") + this->M_FItemPrice.M_ItemMoneyTypeName + TEXT("不够了");
			UWidgetBase::CreateTipWidget(Tip, FVector(1.f, 0.f, 1.f));
		}
		 
	}
}

UEditableTextBox* UUI_ShopPayTip::GetEditableTextBox()
{
	return this->M_PlayerPayCountTextBox;
}

bool UUI_ShopPayTip::Initialize()
{
	if (!Super::Initialize())
		return false;


	this->M_PlayerPayButton = this->GetWidgetComponent<UButton>(this, "Pay");
	this->M_PlayerPayCountTextBox = this->GetWidgetComponent<UEditableTextBox>(this, "Input_PlayerBuyCount");

	if (this->M_PlayerPayButton)
	{
		FScriptDelegate CallFunc;
		CallFunc.BindUFunction(this, "PayButton");
		this->M_PlayerPayButton->OnClicked.Add(CallFunc);
	}

	return true;
}