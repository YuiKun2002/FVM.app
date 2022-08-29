// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerBag.generated.h"

/**
 *
 */

class UCanvasPanel;
class UUniformGridPanel;
class UImage;

UCLASS()
class FVM_API UUI_PlayerBag : public UWidgetBase
{
	GENERATED_BODY()
private:
	//时间分段加载结构
	FTimeClip M_FTimeClip_0;
	//当前选择的界面->索引值 ->装备界面 0  卡片界面1  材料界面2
	uint8 M_Panel_Index;
private:
	UPROPERTY()
		UUniformGridPanel* M_UUniformGridPanel = nullptr;
	//背包选择界面
	UPROPERTY()
		UCanvasPanel* M_PlayerBagSelectPanel = nullptr;
	//背包道具内容
	UPROPERTY()
		UUniformGridPanel* M_PlayerBagContent = nullptr;
private:
	//背包道具缓冲区
	TArray<FEquipmentBase*> M_PlayerBagBuffer;
public:
	//背包名称
	FString M_BagVectorName;
	//背包号
	uint8 M_BagNumber;
public:

	//背包格子文字总容量显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagTotalCount_Text_1;
	//背包格子的文字——1
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_1;
	//背包格子的文字——2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_2;
	//背包格子的文字——3
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_3;
	//背包格子的文字——4
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagCount_Text_4;

	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_1 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_2 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_3 = nullptr;
	//背包格子的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BagCountImage_4 = nullptr;

	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_1 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_2 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_3 = nullptr;
	//背包格子的按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagGridButton_4 = nullptr;

	//背包物品删除按钮
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UButton* M_BagItemUniformRemoveButton = nullptr;

public:
	virtual bool Initialize() override;
	//设置背包格子图片以及文字
	int32 SetBagGridStyle(int32 M_Count, const TArray<FPlayerBagGird>& _BagGrid);
	//清除
	void Clear();
	//加载卡片道具
	void LoadCard(TArray<FItemCard>& _Items_);
	//加载装备道具
	void LoadEquipment(TArray<FEquipmentBase>& _Items_);
	//加载材料道具
	void LoadMaterial(TArray<FMaterialBase>& _Items_);
	//加载装备背包(数组)
	void LoadEquipmentBagToPlayerBag(UUniformGridPanel* _Uniform, int32 _Col);
	//加载背包格子的数据
	TArray<FPlayerBagGird>& LoadBagGridData(uint8 _PanelNumber);
	//购买格子
	void BagGridButton_BuyGrid(uint8 _Panel_Index, uint8 _Gird_Index, int32 _Price);
	//使用格子
	void UseCurrentGrid(uint8 _PanelIndex, uint8 _BagNumber, int32 _Price);
	//添加背包到格子
	void AddBagGrid(int32 _PanelNumber, int32 _BagNumber, TArray<FPlayerBagGird>& _Grid, FEquipmentBase* _Data);
public:
	UFUNCTION()
		void BagGridButton_1_Func();
	UFUNCTION()
		void BagGridButton_2_Func();
	UFUNCTION()
		void BagGridButton_3_Func();
	UFUNCTION()
		void BagGridButton_4_Func();
	UFUNCTION()
		void OpenRemovePanel();
	//关闭选择背包的界面
	UFUNCTION()
		void CloseSelectBagPanel();
public:
	//获取当前界面的索引
	UFUNCTION(BlueprintCallable)
		uint8 GetPanel_Index();
	//加载道具界面->道具
	UFUNCTION(BlueprintCallable)
		void LoadItemsData();
	//加载卡片数据->防御卡界面
	UFUNCTION(BlueprintCallable)
		void LoadCardData();
	//加载材料数据->材料界面 
	UFUNCTION(BlueprintCallable)
		void LoadMaterialData();
	//更新背包容量[返回总容量]
	UFUNCTION(BlueprintCallable)
		int32 UpdateBagCount(uint8 _UpdatePanel, int32 Count);
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void CloseBagPanel();
};
