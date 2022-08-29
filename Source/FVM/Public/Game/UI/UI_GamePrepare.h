// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "UI_GamePrepare.generated.h"

/**
 *   游戏准备界面
 */

class UUniformGridPanel;
class UUI_PlayerBagCardGrid;
class UHorizontalBox;

UCLASS()
class FVM_API UUI_GamePrepare : public UWidgetBase
{
	GENERATED_BODY()
private:
	//时间分段加载结构
	FTimeClip M_FTimeClip;
private:
	//卡片列表
	UPROPERTY()
		UUniformGridPanel* M_CardGrid = nullptr;
	//选择的卡片列表
	UPROPERTY()
		UHorizontalBox* M_SelectCardList = nullptr;
	//背包格子
	UPROPERTY()
		TArray<UUI_PlayerBagCardGrid*> M_BagGrid;
private:
	//选择卡片的数量->Max 14
	UPROPERTY()
		int32 M_SelectCardNum = 0;
public:
	static UUI_GamePrepare* M_GamePrepareStatic;
	//卡片数据
	TArray<FItemCard> M_CardDatas_Copy;
public:
	virtual bool Initialize() override;
	//选择卡片
	UFUNCTION()
		void SelectCard(const FString& _CardName, const FItemCard& _CardData);
	//设置指定的卡片禁用
	UFUNCTION()
		void SetCardEnable(const TArray<FString>& _Names, bool _bEnable);
public:
	//取消卡片选择数量减1
	UFUNCTION(BlueprintCallable)
		void CancelCardNum();
	//获取选卡的数量
	UFUNCTION(BlueprintCallable)
		int32 GetSelectCardNums();
	//从角色背包中加载拥有的所有卡片
	UFUNCTION(BlueprintCallable)
		void LoadCardList();
	//关闭当前界面
	UFUNCTION(BlueprintCallable)
		void CloseCurrentPanel();
};
