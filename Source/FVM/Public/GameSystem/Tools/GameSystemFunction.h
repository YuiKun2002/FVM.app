// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"
#include <Kismet/GameplayStatics.h>

#ifndef PLAYER
#include "GameSystem/PlayerStructManager.h"
#endif

#include "GameSystemFunction.generated.h"

#define F __FUNCTION__

/**
 * 游戏系统的一下函数
 */

 //时间分段结构
struct FTimeClip {
public:
	FTimerHandle M_FTimerHandle;
	int32 M_Count = 0;
public:
	//每获取一次将自动增加一次
	int32 GetCount() {
		if (this->M_Count == 0)
			return this->M_Count;
		this->M_Count++;
		return this->M_Count;
	}
};

UCLASS()
class FVM_API UGameSystemFunction : public UObject
{
	GENERATED_BODY()
public:
	static FDateTime GameSystem_Time;
	static FDateTime GameSystem_Time_Now;
	static FRandomStream Random_Now;
	static bool bEnable;
public:
	//更新时间
	UFUNCTION(BlueprintCallable)
		static void UpdateTime();
	//开启时间更新
	UFUNCTION(BlueprintCallable)
		static void OpenUpdate();
	//关闭更新
	UFUNCTION(BlueprintCallable)
		static void CloseUpdate();
public:
	//清除分时器
	static void ClearTimeClip(FTimerManager& _TimeManager, FTimeClip& _Clip);
	//保持当前角色存档（使用前必须保证角色存档已经被加载过）
	static void SaveCurrentPlayerData();
	//读取当前角色存档（使用前必须保证角色存档已经被加载过）
	static void LoadCurrentPlayerData();
	//从背包获取材料数据指针，通过名称
	static FMaterialBase* GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName);
	//返回材料数组
	static TArray<FMaterialBase*> GetMaterialsArrayByType(TArray<FMaterialBase>& _ArraysSource, EMaterialType _Type);
	//返回卡片等级图片
	static FString GetCardGradeImagePath(int32 _Grade);
	//返回概率结果是否中标
	static bool GetRange(int32 _ValueMaxRange, int32 _EndMaxRange);
	//返回概率结果随机数
	static int32 GetRange_Int(int32 _ValueMaxRange, int32 _EndMaxRange);

	//打印字符串
	static void Log(const FString& _FuncName, const FString& _Content);
public:

	template<class RowType>
	static void GetDataTableRows(const FString& _Path, TArray<RowType*>& _OutRows)
	{
		UDataTable* Table = LoadObject<UDataTable>(0, *_Path);
		Table->GetAllRows("List", _OutRows);
	}

	//删除玩家背包中有待删除的道具
	template<class ItemType>
	static void ClearWaitingItems(TArray<ItemType>& _Arrays)
	{
		TArray<ItemType> _Buff;

		for (ItemType& _Items : _Arrays)
		{
			//将未标记的物品添加到缓冲区
			if (!_Items.bWaitRemove)
				_Buff.Emplace(_Items);
		}
		//清空
		_Arrays.Empty();
		//重新复制新值
		_Arrays = _Buff;
	}
};

namespace Tools {
	template<class RowType>
	inline void GetTableRow(const FString& _Path, TArray<RowType*>& _OutRows)
	{
		UGameSystemFunction::GetDataTableRows<RowType>(_Path, _OutRows);
	}
}
