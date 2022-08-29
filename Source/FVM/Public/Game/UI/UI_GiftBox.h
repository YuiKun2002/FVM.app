// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_GiftBox.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UVerticalBox;
class UUI_PlayerBagEquipmentGrid;

UCLASS()
class FVM_API UUI_GiftBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UButton* M_OpenOnce = nullptr;
	UPROPERTY()
		UButton* M_OpenAll = nullptr;
	UPROPERTY()
		UVerticalBox* M_GiftListBox = nullptr;
private:
	//背包格子装备格子
	UUI_PlayerBagEquipmentGrid* M_UI_PlayerBagEquipmentGrid = nullptr;
	//礼盒数据
	FGiftBox M_GiftBoxData;
	//时间分段加载结构
	FTimeClip M_FTimeClip_0;
	//内容
	TArray<FItemBase> M_ItemBase;
	//文字显示内容
	TArray<FString> M_TipText;
	//文字显示颜色
	TArray<FVector> M_TipTextColor;
public:
	//用于加载
	static FTimeClip M_FTimeClip_1;
public:
	//标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Title;
	//内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Content;
public:
	virtual bool Initialize() override;
public:
	//设置背包装备格子UI
	void SetPlayerBagEquipmentGrid(UUI_PlayerBagEquipmentGrid* _GridUI);
	//获取背包装备格子UI
	UUI_PlayerBagEquipmentGrid* GetPlayerBagEquipmentGrid();
	//设置礼盒名称
	void SetGiftBoxData(const FString& _Name);
	//开启礼盒一次
	UFUNCTION()
		void OpenGiftOnce();
	//全部开启礼盒
	UFUNCTION()
		void OpenGiftAll();
	//解析数据
	UFUNCTION()
		void AnalysisGiftBoxData();
	//创建列表->显示礼盒中所有的奖励
	UFUNCTION()
		void ShowGiftList();
	//显示提升文字
	UFUNCTION()
		void ShowTipText();
public:
	void AnalysisBegin();
	void AnalysisGift(TArray<FGiftBox_Item>& Items);
	void AnalysisGiftBoxMode(int32 _ItemCount, TArray<FGiftBox_Item>& Get_Items, TArray<FGiftBox_Item>& Items);
	void AnalysisItem(TArray<FGiftBox_Item>& Items);
	void CardToBag(FGiftBox_Item& Item);
	void EquipmentToBag(FGiftBox_Item& Item);
	void MaterialToBag(FGiftBox_Item& Item);
	void CoinToBag(FGiftBox_Item& Item);

};
