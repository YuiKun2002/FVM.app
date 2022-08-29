// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "MaterialDataStruct.generated.h"

/**
 *
 */

 //材料-防御卡合成配方数据
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisBlueprint_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//配方结构
	UPROPERTY(EditDefaultsOnly)
		FCardBlueprint M_FMaterial;
};

//材料-防御卡合成配方材料数据
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//配方材料结构
	UPROPERTY(EditDefaultsOnly)
		FCardBlueprintMaterial M_FMaterial;
};

//材料-防御卡转职材料数据
USTRUCT(BlueprintType)
struct FMaterial_CardChangeJobMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//转职材料结构
	UPROPERTY(EditDefaultsOnly)
		FCardChangeJobMaterial M_FMaterial;
};

//材料-香料数据
USTRUCT(BlueprintType)
struct FMaterial_SpicesMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//转职材料结构
	UPROPERTY(EditDefaultsOnly)
		FSpicesMaterial M_FMaterial;
};

//材料-四叶草数据
USTRUCT(BlueprintType)
struct FMaterial_CloverMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//四叶草结构
	UPROPERTY(EditDefaultsOnly)
		FCloverMaterial M_FMaterial;
};

UCLASS()
class FVM_API UMaterialDataStruct : public UObject
{
	GENERATED_BODY()

};
