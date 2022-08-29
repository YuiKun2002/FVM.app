// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "CardDataStruct.generated.h"


//攻击类型卡片结构
USTRUCT(BlueprintType)
struct FCard_ATK_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
		FItemCardATK M_FItemCard;
};

//生产类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Spawn_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
		FItemCardSpawn M_FItemCard;
};

//防御类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Defence_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
		FItemCardDefence M_FItemCard;
};

//辅助类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Aid_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
		FItemCardAid M_FItemCard;
};

//功能类型卡片结构
USTRUCT(BlueprintType)
struct FCard_Function_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//攻击类型的卡片
	UPROPERTY(EditDefaultsOnly)
		FItemCardFunction M_FItemCard;
};

UCLASS()
class FVM_API UCardDataStruct : public UObject
{
	GENERATED_BODY()

};
