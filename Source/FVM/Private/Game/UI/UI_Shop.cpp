// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Shop.h"

#include <Components/CanvasPanel.h>
#include <Components/UniformGridPanel.h>
#include <Components/Button.h>

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"

#include "Game/UI/UI_ShopGrid.h"
#include "Data/CardData/CardDataStruct.h"


bool UUI_Shop::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_SnapUp_0 = this->GetWidgetComponent<UUniformGridPanel>(this, "Left_Items_Content");
	this->M_SnapUp_1 = this->GetWidgetComponent<UUniformGridPanel>(this, "Right_Items_Content");
	this->M_Item_Content = this->GetWidgetComponent<UUniformGridPanel>(this, "Items_Content");

	return true;
}

void UUI_Shop::SnapUpLeft()
{
	UUI_ShopGrid* Grid = CreateWidget<UUI_ShopGrid>(this, LoadClass<UUI_ShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Shop_Grid_Larg.BPUI_Shop_Grid_Larg_C'")));
	Grid->SetItemTipsType(EItemTipsImageType::_ENew);
	this->AddGridToUniform(this->M_SnapUp_0, Grid, (*this->M_ShopItemDatas_0[this->M_FTimeClip_0.M_Count]).M_FItemPrice, this->M_FTimeClip_0, M_FItemLoaddingType_0);
}

void UUI_Shop::SnapUpRight()
{
	UUI_ShopGrid* Grid = CreateWidget<UUI_ShopGrid>(this, LoadClass<UUI_ShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Shop_Grid_Min.BPUI_Shop_Grid_Min_C'")));
	Grid->SetItemTipsType(EItemTipsImageType::_EHalfPirce);
	FItemPrice Price = (*this->M_ShopItemDatas_1[this->M_FTimeClip_1.M_Count]).M_FItemPrice;
	Price.M_ItemPrice = Price.M_ItemPrice / 2;
	this->AddGridToUniform(this->M_SnapUp_1, Grid, Price, this->M_FTimeClip_1, M_FItemLoaddingType_1);
}

void UUI_Shop::ItemRange()
{
	UUI_ShopGrid* Grid = CreateWidget<UUI_ShopGrid>(this, LoadClass<UUI_ShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Shop_Grid_Min.BPUI_Shop_Grid_Min_C'")));
	Grid->SetItemTipsType(EItemTipsImageType::_ESelling);
	this->AddGridToUniform(this->M_Item_Content, Grid, (*this->M_ShopItemDatas_0[this->M_FTimeClip_0.M_Count]).M_FItemPrice, this->M_FTimeClip_0, M_FItemLoaddingType_0);
}

void UUI_Shop::FashionRange()
{
	UUI_ShopGrid* Grid = CreateWidget<UUI_ShopGrid>(this, LoadClass<UUI_ShopGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Shop_Grid_Min.BPUI_Shop_Grid_Min_C'")));
	Grid->SetItemTipsType(EItemTipsImageType::_ESelling);
	this->AddGridToUniform(this->M_Item_Content, Grid, (*this->M_ShopItemDatas_0[this->M_FTimeClip_0.M_Count]).M_FItemPrice, this->M_FTimeClip_0, M_FItemLoaddingType_0);
}

void UUI_Shop::LoadItems(FTimeClip& _Clip, FItemLoaddingType& _LoaddingType, TArray<FItem_Price_Data*>& _Items, UUI_Shop* _ObjectClass, void (UUI_Shop::* Func)())
{
	//判断索引是否有效
	if (_Clip.M_Count < _Items.Num())
	{
		if (_LoaddingType.M_LoadCurrentCount > _LoaddingType.M_LoadCount - 1)
		{
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), _Clip);
		}
		else
		{
			if (_ObjectClass)
			{
				(_ObjectClass->*Func)();
			}
			_Clip.M_Count++;
			_LoaddingType.M_LoadCurrentCount++;
		}
	}
	else {
		UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), _Clip);
	}
}

void UUI_Shop::AddGridToUniform(UUniformGridPanel* _UniformPanel, UUI_ShopGrid* _Grid, FItemPrice _Price, FTimeClip& _Clip, FItemLoaddingType& _LoaddingType)
{
	UWidgetBase::SetImageBrush(_Grid->GetItemHeadImage(), _Price.M_ItemHeadTexture2DPath, FVector(1.f), 1.f, true, FVector(2.f));
	_Grid->M_ItemName = _Price.M_ItemName;
	_Grid->M_ItemTips = _Price.M_ItemTips;
	_Grid->M_ItemPrice = FString(FString::FormatAsNumber(_Price.M_ItemPrice) + _Price.M_ItemMoneyTypeName);
	_Grid->M_FItemPrice = _Price;
	_UniformPanel->AddChildToUniformGrid(_Grid, _LoaddingType.M_LoadCurrentCount / _LoaddingType.M_Col, _LoaddingType.M_LoadCurrentCount - _LoaddingType.M_Col * (_LoaddingType.M_LoadCurrentCount / _LoaddingType.M_Col));
}

void UUI_Shop::LoadSnapUpLeft()
{
	M_FItemLoaddingType_0.Init();
	this->M_ShopItemDatas_0.Empty();
	this->M_SnapUp_0->ClearChildren();

	//获取数据
	this->M_ShopItem = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/ShopData/ShopItemData_0.ShopItemData_0'"));
	this->M_ShopItem->GetAllRows("List", this->M_ShopItemDatas_0);

	M_FItemLoaddingType_0.M_Col = 4;
	M_FItemLoaddingType_0.M_LoadCount = 4;

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);

	M_FTimeClip_0.M_Count = this->M_ShopItemDatas_0.Num() - 4;

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {

		this->LoadItems(this->M_FTimeClip_0, this->M_FItemLoaddingType_0, this->M_ShopItemDatas_0, this, &UUI_Shop::SnapUpLeft);

		}, M_FItemLoaddingType_0.M_LoadDelay, true);

}

void UUI_Shop::LoadSnapUpRight()
{
	M_FItemLoaddingType_1.Init();
	this->M_ShopItemDatas_1.Empty();
	this->M_SnapUp_1->ClearChildren();

	//获取数据
	this->M_ShopItem = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/ShopData/ShopItemData_0.ShopItemData_0'"));
	this->M_ShopItem->GetAllRows("List", this->M_ShopItemDatas_1);

	M_FItemLoaddingType_1.M_Col = 2;
	M_FItemLoaddingType_1.M_LoadCount = this->M_ShopItemDatas_1.Num();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_1.M_FTimerHandle, [&]() {

		this->LoadItems(this->M_FTimeClip_1, this->M_FItemLoaddingType_1, this->M_ShopItemDatas_1, this, &UUI_Shop::SnapUpRight);

		}, M_FItemLoaddingType_1.M_LoadDelay, true);
}

void UUI_Shop::LoadItemRange()
{
	M_FItemLoaddingType_0.Init();
	this->M_ShopItemDatas_0.Empty();
	this->M_Item_Content->ClearChildren();

	//获取数据
	this->M_ShopItem = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/ShopData/ShopItemData_1.ShopItemData_1'"));
	this->M_ShopItem->GetAllRows("List", this->M_ShopItemDatas_0);

	M_FItemLoaddingType_0.M_Col = 5;
	M_FItemLoaddingType_0.M_LoadCount = M_ShopItemDatas_0.Num();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {

		this->LoadItems(this->M_FTimeClip_0, this->M_FItemLoaddingType_0, this->M_ShopItemDatas_0, this, &UUI_Shop::ItemRange);

		}, M_FItemLoaddingType_0.M_LoadDelay, true);
}

void UUI_Shop::LoadFashionRange()
{
	M_FItemLoaddingType_0.Init();
	this->M_ShopItemDatas_0.Empty();
	this->M_Item_Content->ClearChildren();

	//获取数据
	UGameSystemFunction::GetDataTableRows<FItem_Price_Data>("DataTable'/Game/Resource/BP/Data/ShopData/ShopItemData_2.ShopItemData_2'", this->M_ShopItemDatas_0);

	M_FItemLoaddingType_0.M_Col = 5;
	M_FItemLoaddingType_0.M_LoadCount = M_ShopItemDatas_0.Num();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {

		this->LoadItems(this->M_FTimeClip_0, this->M_FItemLoaddingType_0, this->M_ShopItemDatas_0, this, &UUI_Shop::FashionRange);

		}, M_FItemLoaddingType_0.M_LoadDelay, true);
}
