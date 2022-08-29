// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/MouseStructManager.h"
#include "MapDataStruct.generated.h"

/**
 * 老鼠结构  地图结构
 */

 //地图数据结构
USTRUCT(BlueprintType)
struct FGameMapData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	UPROPERTY(EditDefaultsOnly)
		FGameMapStruct M_FGameMapStruct;

	UPROPERTY(EditDefaultsOnly)
		FMouseRounds M_FMouseRounds;
};

UCLASS()
class FVM_API UMapDataStruct : public UObject
{
	GENERATED_BODY()
};
