// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "EquipmentDataSturct.generated.h"

/**
 *
 */

 //装备-背包数据
USTRUCT(BlueprintType)
struct FEquipment_Bag_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FBag M_FEquipment;
};

//装备-玩家装备数据
USTRUCT(BlueprintType)
struct FEquipment_PlayerEquipment_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FPlayerEquipment M_FEquipment;
};

//装备-礼盒数据
USTRUCT(BlueprintType)
struct FEquipment_GiftBox_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FGiftBox M_FEquipment;
};

UCLASS()
class FVM_API UEquipmentDataSturct : public UObject
{
	GENERATED_BODY()

};
