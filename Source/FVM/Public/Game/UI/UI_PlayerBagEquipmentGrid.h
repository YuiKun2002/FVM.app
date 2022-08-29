// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerBagEquipmentGrid.generated.h"

/**
 *
 */

 /*
 这个结构用于接收背包UI中穿过来的数据->如果当前物品是背包那么就会使用到这个数据
 */
 //角色背包结构
struct FPlayerBagData {
public:
	//名称
	FString VectorName;
	//界面索引
	uint8 M_PanelNumber;
	//第几个背包
	uint8 M_GridNumber;
};

class UButton;
class UUI_PlayerBag;

UCLASS()
class FVM_API UUI_PlayerBagEquipmentGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//道具按钮->调出界面
	UPROPERTY()
		UButton* M_UButton = nullptr;
private:
	FString M_ItemName;
public:
	//指向背包的数据
	FEquipmentBase* M_EuipmentData = nullptr;
	//背包UI
	UUI_PlayerBag* M_BagUI = nullptr;
	//背包数据
	FPlayerBagData M_FPlayerBagData;
public:
	//道具显示数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemCountText;
public:
	virtual bool Initialize() override;
	//重新加载背包装备栏
	UFUNCTION()
		void RelaodBagEquipment();
	//背包功能
	UFUNCTION()
		void AttachToBag();
	//创建一个新的礼盒显示界面
	UFUNCTION()
		void CreateNewGiftBox();
	//设置名称
	UFUNCTION()
		void SetItemName(const FString& _Name);
	//获取名称
	UFUNCTION()
		FString GetItemName();
public:
	//获取按钮
	UButton* GetButton();
};
