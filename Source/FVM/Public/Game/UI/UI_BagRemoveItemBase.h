// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_BagRemoveItemBase.generated.h"

/**
 *
 */

class UImage;
class UButton;

UCLASS()
class FVM_API UUI_BagRemoveItemBase : public UWidgetBase
{
	GENERATED_BODY()
private:
	//选择或者取消按钮
	UPROPERTY()
		UButton* M_SelectButton = nullptr;
	//显示选择的状态
	UPROPERTY()
		UImage* M_ShowSelectTag = nullptr;
private:
	//物品索引
	int32 M_ItemIndex;
	//界面索引->决定表示对象从背包中的装备 卡片 材料的那种数组选择
	uint8 M_PanelNumber;
	//是否选择
	bool M_bIsSelectItem = true;
public:
	void SelectItems(bool _Tag);
public:
	//按钮的显示文字
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_SelectButtonText;
public:
	UFUNCTION()
		void Select();
public:
	virtual bool Initialize() override;
	//设置被标记的物品
	void SetTagItems(int32 _ItemIndex, uint8 _PanelNumber);
	//获取选择按钮
	UButton* GetSelectButton();
	//绑定按钮功能
	void BindSelectFunc();
};
