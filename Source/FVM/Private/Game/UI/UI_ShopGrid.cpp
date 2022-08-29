// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_ShopGrid.h"
#include "Game/UI/Tips/UI_ShopPayTip.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

bool UUI_ShopGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_BuyItem_Butt = this->GetWidgetComponent<UButton>(this, "BuyItem");
	this->M_ItemTips_Image = this->GetWidgetComponent<UImage>(this, "Item_Tips");
	this->M_ItemHead_Image = this->GetWidgetComponent<UImage>(this, "Item_Head");


	if (this->M_BuyItem_Butt)
	{
		FScriptDelegate CallFunc;
		CallFunc.BindUFunction(this, "ShowPayPanel");
		this->M_BuyItem_Butt->OnClicked.Add(CallFunc);
	}

	return true;
}

void UUI_ShopGrid::ShowPayPanel()
{
	UUI_ShopPayTip* _PayWidget = CreateWidget<UUI_ShopPayTip>(this, LoadClass<UUI_ShopPayTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_BuyItem_Tip.UI_BuyItem_Tip_C'")));
	_PayWidget->M_ItemName = this->M_FItemPrice.M_ItemName;
	_PayWidget->M_ItemPirceText = FString::FormatAsNumber(this->M_FItemPrice.M_ItemPrice);
	_PayWidget->M_ItemDescribe = this->M_FItemPrice.M_ItemDescirbe;
	_PayWidget->M_ItemMoneyType = this->M_FItemPrice.M_ItemMoneyTypeName;
	//赋值价格数据
	_PayWidget->M_FItemPrice = this->M_FItemPrice;

	_PayWidget->AddToViewport();
}

UButton* UUI_ShopGrid::GetBuyItemButton()
{
	return this->M_BuyItem_Butt;
}

UImage* UUI_ShopGrid::GetItemHeadImage()
{
	return this->M_ItemHead_Image;
}

void UUI_ShopGrid::SetItemTipsType(EItemTipsImageType _EItemTipsImageType)
{
	FString _Texture2DPath;
	bool _IsHave = true;
	switch (_EItemTipsImageType)
	{
	case EItemTipsImageType::_ESelling:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_6.T_Shop_6'";
		break;
	case EItemTipsImageType::_ENew:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_7.T_Shop_7'";
		break;
	case EItemTipsImageType::_EHalfPirce:
		_Texture2DPath = "Texture2D'/Game/Resource/Texture/UI/Game/Shop/T_Shop_8.T_Shop_8'";
		break;
	case EItemTipsImageType::_ENone:
		_IsHave = false;
		break;
	}

	if (_IsHave)
	{
		UWidgetBase::SetImageBrush(this->M_ItemTips_Image, _Texture2DPath);
	}
	else {
		this->M_ItemTips_Image->SetVisibility(ESlateVisibility::Hidden);
	}
}
