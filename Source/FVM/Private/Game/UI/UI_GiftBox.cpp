// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GiftBox.h"
#include "Game/UI/UI_TableTextBlock.h"
#include "Game/UI/UI_PlayerBagEquipmentGrid.h"
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/VerticalBox.h>
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"
#include "Data/EquipmentDataSturct.h"
#include <Kismet/KismetTextLibrary.h>

FTimeClip UUI_GiftBox::M_FTimeClip_1;

bool UUI_GiftBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_OpenOnce = this->GetWidgetComponent<UButton>(this, "Open_Once");
	this->M_OpenAll = this->GetWidgetComponent<UButton>(this, "Open_All");
	this->M_GiftListBox = this->GetWidgetComponent<UVerticalBox>(this, "VerticalBox_33");

	this->M_OpenOnce->OnClicked.AddDynamic(this, &UUI_GiftBox::OpenGiftOnce);
	this->M_OpenAll->OnClicked.AddDynamic(this, &UUI_GiftBox::OpenGiftAll);

	return true;
}

void UUI_GiftBox::SetPlayerBagEquipmentGrid(UUI_PlayerBagEquipmentGrid* _GridUI)
{
	this->M_UI_PlayerBagEquipmentGrid = _GridUI;
}

UUI_PlayerBagEquipmentGrid* UUI_GiftBox::GetPlayerBagEquipmentGrid()
{
	return this->M_UI_PlayerBagEquipmentGrid;
}

void UUI_GiftBox::SetGiftBoxData(const FString& _Name)
{
	UGameSystemFunction::OpenUpdate();

	bool Result = false;

	//加载礼盒数据
	TArray<FGiftBox> Item;
	UPlayerStructManager::GetEquipmentRowDatas<FEquipment_GiftBox_Data, FGiftBox>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_Gift), Item);
	UPlayerStructManager::GetEquipmentArraysData<FGiftBox>(_Name, Item, this->M_GiftBoxData, Result);

	if (Result)
	{
		//设置标题
		this->M_Title = this->M_GiftBoxData.ItemName;
		//设置内容
		this->M_Content = this->M_GiftBoxData.ItemDescrible;
		//加载列表
		this->ShowGiftList();
	}

}

void UUI_GiftBox::OpenGiftOnce()
{

	this->AnalysisBegin();

	//礼盒数量减一
	FEquipmentBase* Item = UPlayerStructManager::GetItemFromBag<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment, this->M_Title);

	if (Item)
	{
		Item->M_Count--;
		if (Item->M_Count <= 0)
			Item->bWaitRemove = true;
	}

	UGameSystemFunction::ClearWaitingItems<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);

	UGameSystemFunction::SaveCurrentPlayerData();
	UGameSystemFunction::LoadCurrentPlayerData();

	this->GetPlayerBagEquipmentGrid()->RelaodBagEquipment();

	this->ShowTipText();
}

void UUI_GiftBox::OpenGiftAll()
{
	//礼盒数量减一
	FEquipmentBase* Item = UPlayerStructManager::GetItemFromBag<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment, this->M_Title);

	if (!Item)
		return;

	int32 _Count = Item->M_Count;

	//销毁礼盒
	Item->bWaitRemove = true;
	UGameSystemFunction::ClearWaitingItems<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment);



	for (int32 i = 0; i < _Count; i++)
	{
		this->AnalysisBegin();
	}

	UGameSystemFunction::SaveCurrentPlayerData();
	UGameSystemFunction::LoadCurrentPlayerData();

	this->GetPlayerBagEquipmentGrid()->RelaodBagEquipment();

	this->ShowTipText();
}

void UUI_GiftBox::AnalysisGiftBoxData()
{
	//剩下的礼盒
	TArray<FGiftBox_Item> GiftBox_Items;

	//解析礼盒内容
	for (auto Item : this->M_GiftBoxData.M_GiftContent)
	{
		if (Item.M_SelectCurrentTypeAll)
		{
			//如果当前开启将所有道具全部选择
			switch (Item.M_ItemType)
			{
			case EGiftBox_Item_Type::E_Card:UPlayerStructManager::GetAllCardName(this->M_ItemBase); break;
			case EGiftBox_Item_Type::E_Equipment:UPlayerStructManager::GetAllEquipmentName(this->M_ItemBase); break;
			case EGiftBox_Item_Type::E_Material:UPlayerStructManager::GetAllMaterial<FItemBase>(this->M_ItemBase); break;
			case EGiftBox_Item_Type::E_Coin:break;
			}
		}
		else {
			//添加非全选物品
			GiftBox_Items.Emplace(Item);
		}
	}


	for (auto& Gift : GiftBox_Items)
	{
		if (!Gift.M_SelectCurrentTypeAll)
		{
			FItemBase Item;
			float Value = ((Gift.M_GiftNumber + 0.0f) / this->M_GiftBoxData.M_RandomMaxNumber) * 100.0f;
			TEnumAsByte<ERoundingMode> E;
			FText _text = UKismetTextLibrary::Conv_FloatToText(Value, E, false, false, 1, 100, 0, 3);
			Item.ItemName = Gift.M_ItemName + " " + _text.ToString() + "%";
			this->M_ItemBase.Emplace(Item);
		}
	}

}

void UUI_GiftBox::ShowGiftList()
{

	this->M_GiftListBox->ClearChildren();
	this->M_ItemBase.Empty();

	this->AnalysisGiftBoxData();

	//this->M_ItemBase.Emplace(Item);

//分时加载奖励列表
	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < M_ItemBase.Num())
		{

			UUI_TableTextBlock* TextBlock = CreateWidget<UUI_TableTextBlock>(this, LoadClass<UUI_TableTextBlock>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_TableTextBlock.BPUI_TableTextBlock_C'")));
			TextBlock->SetText(this->M_ItemBase[this->M_FTimeClip_0.M_Count].ItemName);
			this->M_GiftListBox->AddChildToVerticalBox(TextBlock);

			this->M_FTimeClip_0.M_Count++;
		}
		else { UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0); }
		}, 0.02f, true);
}

void UUI_GiftBox::ShowTipText()
{
	//分时加载文字

	UGameSystemFunction::ClearTimeClip(UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager(), UUI_GiftBox::M_FTimeClip_1);

	//UE_LOG(LogTemp, Error, TEXT("%d"), this->M_TipText.Num());

	//提示文字
	static TArray<FString> Tips;
	static TArray<FVector> TipsColor;
	//先清空文字
	Tips.Empty();
	TipsColor.Empty();
	//赋值文字
	Tips = this->M_TipText;
	TipsColor = this->M_TipTextColor;
	//清空文字
	this->M_TipText.Empty();
	this->M_TipTextColor.Empty();

	UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager().SetTimer(UUI_GiftBox::M_FTimeClip_1.M_FTimerHandle, [&]() {
		//判断索引是否有效
		if (this->UUI_GiftBox::M_FTimeClip_1.M_Count < Tips.Num())
		{
			UWidgetBase::CreateTipWidget(Tips[this->UUI_GiftBox::M_FTimeClip_1.M_Count], TipsColor[this->UUI_GiftBox::M_FTimeClip_1.M_Count]);
			this->UUI_GiftBox::M_FTimeClip_1.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(UFVMGameInstance::GetFVMGameInstance()->GetWorld()->GetTimerManager(), UUI_GiftBox::M_FTimeClip_1);
			//先清空文字
			Tips.Empty();
			TipsColor.Empty();
		}
		}, 0.15f, true);
}



void UUI_GiftBox::AnalysisBegin()
{
	//等待抽奖的物品
	TArray<FGiftBox_Item> Items;

	this->AnalysisGift(Items);

	//当数量大于0 则该抽奖
	int32 ItemCount = Items.Num();

	if (ItemCount > 0)
	{
		//存储通过抽奖或者其他方式获得的物品
		TArray<FGiftBox_Item> _Items_;
		//解析抽奖模式(抽奖或者不抽奖)
		this->AnalysisGiftBoxMode(ItemCount, Items, _Items_);
		//处理最后一次礼盒
		this->AnalysisItem(_Items_);
	}
}

void UUI_GiftBox::AnalysisGift(TArray<FGiftBox_Item>& Items)
{
	for (FGiftBox_Item& Item : this->M_GiftBoxData.M_GiftContent)
	{
		//全部都选择，并且没有开启随机模式->那么将获得所有道具
		if (Item.M_SelectCurrentTypeAll && !this->M_GiftBoxData.M_bEnableRandomMode)
		{
			//如果当前开启将所有道具全部选择
			switch (Item.M_ItemType)
			{
			case EGiftBox_Item_Type::E_Card:UPlayerStructManager::CreateAllCardToBag(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager(), Item.M_CardGrade); break;
			case EGiftBox_Item_Type::E_Equipment:UPlayerStructManager::CreateAllEquipmentToBag(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()); break;
			case EGiftBox_Item_Type::E_Material: {UPlayerStructManager::GetAllMaterial<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material, Item.M_ItemCount); }break;
			case EGiftBox_Item_Type::E_Coin:break;
			}

			//将文字加入到列表准备一次显示
			FString Text = TEXT("恭喜你获得全部道具");
			this->M_TipText.Emplace(Text);
			this->M_TipTextColor.Emplace(FVector(1.f, 0.2f, 0.5f));
		}
		else {
			//将其他剩余物品全部加入到列表
			Items.Emplace(Item);
		}
	}
}

void UUI_GiftBox::AnalysisGiftBoxMode(int32 _ItemCount, TArray<FGiftBox_Item>& Get_Items, TArray<FGiftBox_Item>& Items)
{
	//开启随机模式
	if (this->M_GiftBoxData.M_bEnableRandomMode)
	{
		//当前获取的个数
		int32 GetCount = 0;

		//抽取道具次数
		for (; GetCount < this->M_GiftBoxData.M_GetGiftMaxCount;)
		{
			//一共的道具量
			for (int32 _CurrentItem = 0; _CurrentItem < _ItemCount; _CurrentItem++)
			{
				//解析当前道具内容
				if (UGameSystemFunction::GetRange(Get_Items[_CurrentItem].M_GiftNumber, this->M_GiftBoxData.M_RandomMaxNumber))
				{
					//抽奖次数叠加
					GetCount++;
					Items.Emplace(Get_Items[_CurrentItem]);
					break;
				}
			}

		}
	}
	else {
		//没有开启随机模式->不会收到礼盒个数的限制
		for (int32 i = 0; i < _ItemCount; i++)
		{
			Items.Emplace(Get_Items[i]);
		}
	}
}

void UUI_GiftBox::AnalysisItem(TArray<FGiftBox_Item>& Items)
{
	//解析道具
	for (FGiftBox_Item& ItemData : Items)
	{
		switch (ItemData.M_ItemType)
		{
		case EGiftBox_Item_Type::E_Card: this->CardToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Equipment:this->EquipmentToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Material:this->MaterialToBag(ItemData); break;
		case EGiftBox_Item_Type::E_Coin:this->CoinToBag(ItemData); break;
		}
	}
}

void UUI_GiftBox::CardToBag(FGiftBox_Item& Item)
{
	FItemCard Card;

	//是否选择了全部并且开启了随机模式 （在所有的道具中选择一个）
	if (Item.M_SelectCurrentTypeAll && this->M_GiftBoxData.M_bEnableRandomMode)
	{
		TArray<FItemCard> Cards;
		UPlayerStructManager::GetAllCardsData(Cards, Item.M_CardGrade);
		//开始抽取
		int32 _Index = UGameSystemFunction::GetRange_Int(0, Cards.Num() - 1);
		Card = Cards[_Index];
	}
	else {
		UPlayerStructManager::SearchCardFromDataTable(Item.M_ItemName, Card);
		Card.M_CardGrade = Item.M_CardGrade;
	}

	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(Card);
	// 1.f 0.2f 0.f
	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Card.ItemName;
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(1.f, 0.2f, 0.f));
}

void UUI_GiftBox::EquipmentToBag(FGiftBox_Item& Item)
{
	FEquipmentBase Equipment;
	UPlayerStructManager::SearchEquipmentFromDataTable(Item.M_ItemName, Equipment);
	Equipment.M_Count = Item.M_ItemCount;
	//获取道具
	FEquipmentBase* _Item = UPlayerStructManager::GetItemFromBag<FEquipmentBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment, Equipment.ItemName);
	//如果从背包中寻找到则直接添加
	if (_Item && _Item->M_IsOverlap)
	{
		_Item->M_Count += Item.M_ItemCount;
	}
	else {
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment.Emplace(Equipment);
	}
	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Equipment.ItemName;
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(1.f, 0.f, 1.f));
}

void UUI_GiftBox::MaterialToBag(FGiftBox_Item& Item)
{
	FMaterialBase Materials;

	//是否选择了全部并且开启了随机模式 （在所有的道具中选择一个）
	if (Item.M_SelectCurrentTypeAll && this->M_GiftBoxData.M_bEnableRandomMode)
	{
		TArray<FMaterialBase> _Item;
		UPlayerStructManager::GetAllMaterial(_Item, Item.M_ItemCount);
		//开始抽取
		int32 _Index = UGameSystemFunction::GetRange_Int(0, _Item.Num() - 1);
		Materials = _Item[_Index];
	}
	else {
		UPlayerStructManager::SearchMaterailFromDataTable(Item.M_ItemName, Materials);
		Materials.M_Count = Item.M_ItemCount;
	}

	//获取道具
	FMaterialBase* _Item = UPlayerStructManager::GetItemFromBag<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material, Materials.ItemName);
	if (_Item)
	{
		_Item->M_Count += Materials.M_Count;
	}
	else {
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material.Emplace(Materials);
	}
	//将文字加入到列表准备一次显示
	FString Text = TEXT("恭喜你获得:") + Materials.ItemName;
	this->M_TipText.Emplace(Text);
	this->M_TipTextColor.Emplace(FVector(1.f));
}

void UUI_GiftBox::CoinToBag(FGiftBox_Item& Item)
{
}
