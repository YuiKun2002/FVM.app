// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_Shop.generated.h"


class UButton;
class UCanvasPanel;
class UUniformGridPanel;
class UUI_ShopGrid;


struct FItemLoaddingType
{
public:
	int32 M_LoadCount = 0;
	int32 M_LoadCurrentCount = 0;
	int32 M_Col = 0;
	float M_LoadDelay = 0.02f;
public:
	void Init() {
		this->M_LoadCount = 0;
		this->M_LoadCurrentCount = 0;
		this->M_Col = 0.f;
		this->M_LoadDelay = 0.02f;
	}
};

UCLASS()
class FVM_API UUI_Shop : public UWidgetBase
{
	GENERATED_BODY()
private:
	//时间分段加载结构
	FTimeClip M_FTimeClip_0;
	FTimeClip M_FTimeClip_1;
	//加载结构
	FItemLoaddingType M_FItemLoaddingType_0;
	FItemLoaddingType M_FItemLoaddingType_1;
	//数据表数据
	TArray<FItem_Price_Data*> M_ShopItemDatas_0;
	TArray<FItem_Price_Data*> M_ShopItemDatas_1;
	//数据表
	UPROPERTY()
		UDataTable* M_ShopItem = nullptr;
private:
	UPROPERTY()
		UUniformGridPanel* M_SnapUp_0 = nullptr;
	UPROPERTY()
		UUniformGridPanel* M_SnapUp_1 = nullptr;
	UPROPERTY()
		UUniformGridPanel* M_Item_Content = nullptr;
private:
	int32 M_ItemMoney = 0;
	uint8 M_ItemMoneyType = 0;
public:
	virtual bool Initialize() override;

	//抢购区域加载的物品
	void SnapUpLeft();
	//抢购区域加载的物品
	void SnapUpRight();
	//道具区域加载的物品
	void ItemRange();
	//时装区域加载的物品
	void FashionRange();

	void LoadItems(FTimeClip& _Clip, FItemLoaddingType& _LoaddingType, TArray<FItem_Price_Data*>& _Items, UUI_Shop* _ObjectClass, void (UUI_Shop::* Func)());

	void AddGridToUniform(UUniformGridPanel* _UniformPanel, UUI_ShopGrid* _Grid, FItemPrice _Price, FTimeClip& _Clip, FItemLoaddingType& _LoaddingType);
public:

	//加载抢购区域的物品（左边）
	UFUNCTION(BlueprintCallable)
		void LoadSnapUpLeft();

	//加载抢购区域的物品（右边）
	UFUNCTION(BlueprintCallable)
		void LoadSnapUpRight();

	//加载道具区域的物品
	UFUNCTION(BlueprintCallable)
		void LoadItemRange();

	//加载时装区域的物品
	UFUNCTION(BlueprintCallable)
		void LoadFashionRange();
};
