// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagRemove.h"

#include "Game/UI/MyUI_BagRemove_Card.h"
#include "Game/UI/UI_BagRemove_Equipment.h"
#include "Game/UI/UI_PlayerBag.h"

#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Components/VerticalBox.h>

bool UUI_PlayerBagRemove::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_ItemsList = this->GetWidgetComponent<UVerticalBox>(this, "SearchItemsList");
	this->M_InputItemNameBox = this->GetWidgetComponent<UEditableTextBox>(this, "Input_SearchItemBox");
	this->M_SearchButton = this->GetWidgetComponent<UButton>(this, "Serach_Butt");
	this->M_RemoveButton = this->GetWidgetComponent<UButton>(this, "Remove_Item");
	this->M_ClosePanelButton = this->GetWidgetComponent<UButton>(this, "CloseCurrentPanel");


	//绑定关闭界面
	this->M_ClosePanelButton->OnClicked.AddDynamic(this, &UUI_PlayerBagRemove::CloseCurrentPanel);
	this->M_RemoveButton->OnClicked.AddDynamic(this, &UUI_PlayerBagRemove::Remove);

	return true;
}

void UUI_PlayerBagRemove::WidgetAddBox(const FString& _ItemName, const FString& _TexturePath, bool _bOverlaped, int32 _Count, int32 _Index, uint8 _PanelNum, bool _bSelect)
{
	UUI_BagRemove_Equipment* Widget = CreateWidget<UUI_BagRemove_Equipment>(this, LoadClass<UUI_BagRemove_Equipment>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagItemsEquipmentView.BP_PlayerBagItemsEquipmentView_C'")));
	Widget->SetEquipment(_TexturePath, _bOverlaped, _Count);
	Widget->M_EquipmentName = _ItemName;
	Widget->SetTagItems(_Index, _PanelNum);
	Widget->SelectItems(_bSelect);
	Widget->BindSelectFunc();
	this->M_ItemsList->AddChildToVerticalBox(Widget);
}

void UUI_PlayerBagRemove::WidgetAddBox(const FString& _ItemName, const FString& _TexturePath, int32 _CardGrade, int32 _Index, uint8 _PanelNum, bool _bSelect)
{
	UMyUI_BagRemove_Card* Widget = CreateWidget<UMyUI_BagRemove_Card>(this, LoadClass<UMyUI_BagRemove_Card>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagItemsCardView.BP_PlayerBagItemsCardView_C'")));
	Widget->SetCard(_TexturePath, _CardGrade);
	Widget->M_CardName = _ItemName;
	Widget->SetTagItems(_Index, _PanelNum);
	Widget->SelectItems(_bSelect);
	Widget->BindSelectFunc();
	this->M_ItemsList->AddChildToVerticalBox(Widget);
}

void UUI_PlayerBagRemove::SearchBag_Card()
{
	this->M_ItemsList->ClearChildren();

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(1); return;
	}

	TArray<FItemCard>& _Card = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;

	int32 _SearchMaxCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);
	int32 _Index = 0;

	UE_LOG(LogTemp, Error, TEXT("%d -- "), _SearchMaxCount);

	for (FItemCard& _Item : _Card)
	{
		//限定搜索个数
		if (_Index >= _SearchMaxCount)
			break;

		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()))
		{
			this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, _Item.M_CardGrade, _Index, 1, true);
		}

		_Index++;
	}
}

void UUI_PlayerBagRemove::SearchBag_Equipment()
{
	this->M_ItemsList->ClearChildren();

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(0); return;
	}

	TArray<FEquipmentBase>& _Euipment = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment;

	int32 _SearchMaxCount = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0);

	int32 _Index = 0;

	for (FEquipmentBase& _Item : _Euipment)
	{
		//限制搜索个数
		if (_Index >= _SearchMaxCount)
			break;

		//搜到之后只能搜索到未使用的装备对象
		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()) && !_Item.M_Used)
		{
			this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, _Item.M_IsOverlap, _Item.M_Count, _Index, 0, true);
		}
		_Index++;
	}
}

void UUI_PlayerBagRemove::SearchBag_Materials()
{
	this->M_ItemsList->ClearChildren();

	//如果没有输入任何物品则全部加载
	if (this->M_InputItemNameBox->GetText().ToString().Equals(""))
	{
		this->SearchBag(2); return;
	}

	TArray<FMaterialBase>& _Materials = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;

	int32 _Index = 0;
	for (FMaterialBase& _Item : _Materials)
	{
		//搜到之后只能搜索到未使用的装备对象
		if (_Item.ItemName.Contains(this->M_InputItemNameBox->GetText().ToString()))
		{
			this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, true, _Item.M_Count, _Index, 2, true);
		}
		_Index++;
	}

}

void UUI_PlayerBagRemove::CloseCurrentPanel()
{
	this->RemoveFromParent();

	//将选择的标记全部取消

	if (this->M_PanelNumber == 0)
		for (FEquipmentBase& _Equipment : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
			_Equipment.bWaitRemove = false;

	if (this->M_PanelNumber == 1)
		for (FItemCard& _Card : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card)
			_Card.bWaitRemove = false;

	if (this->M_PanelNumber == 2)
		for (FMaterialBase& _Materials : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material)
			_Materials.bWaitRemove = false;
}

void UUI_PlayerBagRemove::SearchBag(uint8 _PanelNumber)
{
	//this->M_ItemsList->ClearChildren();

	int32 _SearchMaxCount_0 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(0);
	int32 _SearchMaxCount_1 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1);
	int32 _SearchMaxCount_2 = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(2);

	int32 _Index = 0;

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 0)
	{
		TArray<FEquipmentBase>& _Euipment = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment;
		for (FEquipmentBase& _Item : _Euipment)
		{
			if (_Index >= _SearchMaxCount_0)
				return;
			//搜到之后只能搜索到未使用的装备对象
			if (!_Item.M_Used) { this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, _Item.M_IsOverlap, _Item.M_Count, _Index, 0, false); }
			_Index++;
		}
	}

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 1)
	{
		TArray<FItemCard>& _Card = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
		for (FItemCard& _Item : _Card)
		{
			if (_Index >= _SearchMaxCount_1)
				return;
			this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, _Item.M_CardGrade, _Index, 1, false);
			_Index++;
		}
	}

	//根据不同界面加载所有的选项但是不标记
	if (_PanelNumber == 2)
	{
		TArray<FMaterialBase>& _Materials = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
		for (FMaterialBase& _Item : _Materials)
		{
			if (_Index >= _SearchMaxCount_2)
				return;
			this->WidgetAddBox(_Item.ItemName, _Item.ItemTexturePath, true, _Item.M_Count, _Index, 2, false);
			_Index++;
		}
	}

}

UButton* UUI_PlayerBagRemove::GetSearchButton()
{
	return this->M_SearchButton;
}

void UUI_PlayerBagRemove::Remove()
{
	if (this->M_PanelNumber == 0)
	{
		this->M_UIBag->Clear();

		this->Remove_Item<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);

		this->M_UIBag->LoadItemsData();
	}

	if (this->M_PanelNumber == 1)
	{
		this->M_UIBag->Clear();

		this->Remove_Item<FItemCard>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card);

		//加载数据
		this->M_UIBag->LoadCardData();
	}

	if (this->M_PanelNumber == 2)
	{
		this->M_UIBag->Clear();

		this->Remove_Item<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);

		//保存数据
		UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);

		this->M_UIBag->LoadMaterialData();
	}
	this->RemoveFromParent();
}
