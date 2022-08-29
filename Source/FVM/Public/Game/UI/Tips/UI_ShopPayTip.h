// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "Data/ShopItemPriceStruct.h"
#include "UI_ShopPayTip.generated.h"

class UTextBlock;
class UButton;
class UEditableTextBox;

UCLASS()
class FVM_API UUI_ShopPayTip : public UWidgetBase
{
	GENERATED_BODY()
private:
	//输入的需要支付的物品个数
	UPROPERTY()
		UEditableTextBox* M_PlayerPayCountTextBox = nullptr;
	//支付按钮
	UPROPERTY()
		UButton* M_PlayerPayButton = nullptr;
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName;
	//物品描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemDescribe;
	//物品价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemPirceText;
	//物品价格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemMoneyType;
public:
	//数据
	FItemPrice M_FItemPrice;
public:
	//支付
	UFUNCTION()
		void PayButton();
	//获取输入框
	UFUNCTION()
		UEditableTextBox* GetEditableTextBox();
public:
	virtual bool Initialize() override;
};
