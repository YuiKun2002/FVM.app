// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "UI_PlayerBagMaterialGrid.generated.h"

/**
 *
 */

class UButton;
class UUI_PlayerSynthesis;

UCLASS()
class FVM_API UUI_PlayerBagMaterialGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//合成屋的UI
	UUI_PlayerSynthesis* M_UUI_PlayerSynthesis = nullptr;
	//物品数据
	FMaterialBase* M_MaterialBaseData = nullptr;
private:
	UPROPERTY()
		UButton* M_Button = nullptr;
public:
	//显示的道具数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CountText;
	//道具基本数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMaterialBase M_FMaterialBaseData;
public:
	virtual bool Initialize() override;
	//设置合成屋UI的指向
	void SetUI_PlayerSynthesis(UUI_PlayerSynthesis* _UUI_PlayerSynthesis);
	//设置物品基本数据
	void SetMaterialData(FMaterialBase* _Data);
	//获取物品基本数据
	FMaterialBase* const GetMaterialData();
public:
	//获取按钮
	UFUNCTION(BlueprintCallable)
		UButton* GetButton();
	//更新物品显示的数量
	UFUNCTION(BlueprintCallable)
		void UpdateMaterialsShowCount(FString _CountText);
public:
	/*----------------------------------------卡片制作----------------------------------------*/
//将合成配方的信息存储，并且修改合成界面的某些显示图片
	UFUNCTION()
		void AddSynthesisBlueprint();
	//添加到合成屋香料槽位
	UFUNCTION()
		void AddSynthesisSpicesSlot();
	//添加到卡片强化四叶草槽位
	UFUNCTION()
		void AddUpGradeCardCloverSlot();
	/*----------------------------------------卡片制作----------------------------------------*/
};
