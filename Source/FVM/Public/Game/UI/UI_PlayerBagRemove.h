// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "UI_PlayerBagRemove.generated.h"

/**
 *
 */

class UUI_PlayerBag;
class UVerticalBox;
class UButton;
class UEditableTextBox;
class UUI_BagRemove_Equipment;

UCLASS()
class FVM_API UUI_PlayerBagRemove : public UWidgetBase
{
	GENERATED_BODY()
private:
	//输出物品列表
	UPROPERTY()
		UVerticalBox* M_ItemsList = nullptr;
	//搜索物品名称
	UPROPERTY()
		UEditableTextBox* M_InputItemNameBox = nullptr;
	//搜索按钮
	UPROPERTY()
		UButton* M_SearchButton = nullptr;
	//删除按钮
	UPROPERTY()
		UButton* M_RemoveButton = nullptr;
	//关闭按钮
	UPROPERTY()
		UButton* M_ClosePanelButton = nullptr;
public:
	//界面序号
	uint8 M_PanelNumber;
	//背包UI
	UUI_PlayerBag* M_UIBag = nullptr;
public:
	virtual bool Initialize() override;
	//将界面添加到背包
	void WidgetAddBox(const FString& _ItemName, const FString& _TexturePath, bool _bOverlaped, int32 _Count, int32 _Index, uint8 _PanelNum, bool _bSelect);
	void WidgetAddBox(const FString& _ItemName, const FString& _TexturePath, int32 _CardGrade, int32 _Index, uint8 _PanelNum, bool _bSelect);
	//搜索背包数据
	void SearchBag(uint8 _PanelNumber);
	//获取搜索按钮
	UButton* GetSearchButton();
	//销毁卡片
	UFUNCTION()
		void Remove();
public:
	//搜索卡片数据
	UFUNCTION()
		void SearchBag_Card();

	//搜索装备数据
	UFUNCTION()
		void SearchBag_Equipment();

	//搜索材料数据
	UFUNCTION()
		void SearchBag_Materials();

	//关闭当前界面
	UFUNCTION()
		void CloseCurrentPanel();
public:

	template <class ItemStruct>
	void Remove_Item(TArray<ItemStruct>& _Arrays)
	{
		//获取没有被标记的卡片
		TArray<ItemStruct> Card;

		for (TIndexedContainerIterator<const TArray<ItemStruct>, const ItemStruct, int32> PP = _Arrays.CreateConstIterator(); PP; PP++)
			if (!(*PP).bWaitRemove)
				Card.Emplace((*PP));

		_Arrays.Empty();
		//复制新的卡片数据
		_Arrays = Card;

		//保存数据
		UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
	}
};
