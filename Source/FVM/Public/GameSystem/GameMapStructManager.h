// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameMapStructManager.generated.h"

UENUM(BlueprintType)
enum class EMesheColMax : uint8 {
	EColMeshMin = 0 UMETA(DisplayName = "MesheCountMin"),
	EColMeshMax = 15 UMETA(DisplayName = "MesheCountMax"),
};

UENUM(BlueprintType)
enum class EMesheRowMax : uint8 {
	ERowMeshMin = 0 UMETA(DisplayName = "MesheCountMin"),
	ERowMeshMax = 15 UMETA(DisplayName = "MesheCountMax"),
};

//游戏地图结构
USTRUCT(BlueprintType)
struct FGameMapStruct
{
	GENERATED_USTRUCT_BODY();
public:
	//地图名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MapName = FString(TEXT("曲奇岛"));
	//多少列网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ColMesheCount = 1;
	//启用用了多少列
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_EnableColMesheCount = 1;
	//多少行网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_RowMesheCount = 1;
	//启用了多少行网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_EnableRowMesheCount = 1;
	//网格类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineType> M_GradeLineType;
	//最大列网格数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMesheColMax M_MesheColMax = EMesheColMax::EColMeshMax;
	//最大行网格数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMesheRowMax M_MesheRowMax = EMesheRowMax::ERowMeshMax;
	//第一行网格的坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector2D M_FirstRowMesheLocation = FVector2D(-135.f, 260.f);
	//列间距
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ColMargin = 57.5f;
	//默认统一行间距
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_DefRowUniformMargin = 61.f;
	//每行行间距
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> M_RowMargin;
	//默认的老鼠配置名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseStructConfigName = FString("DefGameMouseStructConfig");
	//地图的背景名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MapBGName = FString("T_Map_0_0_Sprite");
	//开场背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BeginMusic = FString("Day_0_0");
	//精英背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Begin_UpRound_Music = FString("Day_0_1");
	//boss出场背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Boss_Music = FString("Day_0_2");
};

UCLASS()
class FVM_API UGameMapStructManager : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameMapStruct M_Data;
};
