// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameConfigManager.generated.h"

//音乐音量
USTRUCT(BlueprintType)
struct FMusic {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BGM_Value = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BGM_S_Value = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Operating_BGM = FString("Def_Click");
};

UCLASS()
class FVM_API UGameConfigManager : public USaveGame
{
	GENERATED_BODY()
public:
	//角色所有保存的存档名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> M_PlayerStructManagerName;
	//角色的性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<uint8> M_PlayerStructManagerSex;
	//角色准备环节选择的卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemCard> M_PlayerGamePrepareSelectCardData;
	//角色进入地图的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerInGameMapName;
	//当前选择的角色
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CurrentPlayerName = FString("");
	//音乐音效 声音大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMusic M_FMusic;

};
