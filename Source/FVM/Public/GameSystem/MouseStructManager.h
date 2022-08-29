// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MouseStructManager.generated.h"


//老鼠结构
USTRUCT(BlueprintType)
struct FMouseBase {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseName;
	//老鼠Actor资源名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemTarget_ActorFileName;
	//老鼠的血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MouseHP;
	//身体生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_BodyHP;
	//老鼠头部防御血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MouseHeadDefenceHP;
	//老鼠前防御血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MouseFrontDefenceHP;
	//老鼠的速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MouseSpeed;
	//老鼠的攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_MouseATK;
	//老鼠的动画资源路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseAnimPath;
};


//生成后的老鼠路线设置
USTRUCT(BlueprintType)
struct FMouseLineSetting {
	GENERATED_USTRUCT_BODY()
public:
	//默认
	FMouseLineSetting() {}
	//初始化
	FMouseLineSetting(const FString& _Name, TArray<int32>& _SpawnLine)
		: M_MouseRefPath(_Name), M_MouseSpawnLine(_SpawnLine) {}
public:
	//需要加载的老鼠资源
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseRefPath;
	//指定在第一条路线生成当前老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<int32> M_MouseSpawnLine;
};

USTRUCT(BlueprintType)
struct FMouseRound {
	GENERATED_USTRUCT_BODY()
public:
	//当前回合中一共有多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MouseMax = 20;
	//最多一次性出现的最大个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MouseMaxCount = 2;
	//最多一次性出现的最小个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MouseMinCount = 1;
	//当前回合中每默认2s生成最小和最大值的老鼠个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnTime = 2.f;
	//当回合开始时的一瞬间生成多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_RoundBeginSpawnMaxCount = 4;
	//回合生效延迟(默认5s之后回合将立刻生效)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_RoundBeginDelay = 5.f;
	//如果开启当老鼠为0时,才会进行下一波操作
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsEnableMouseCountZero = true;
	//当关闭老鼠为0时，会默认开启30m之后自动进入下一波
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_NextRoundTime = 30.f;
	//老鼠回合结束的启用延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_RoundEndDelay = 5.f;
	//这个回合是否需要Key才能启动
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Key = false;
	//老鼠的细节设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMouseLineSetting> M_MouseLineSetting;
};

//老鼠回合结构
USTRUCT(BlueprintType)
struct FMouseRounds
{
	GENERATED_USTRUCT_BODY();
public:
	//要保存的老鼠回合文件名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseRoundStructName;
	//老鼠的所有回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMouseRound> M_MouseRound;
};

UCLASS()
class FVM_API UMouseStructManager : public USaveGame
{
	GENERATED_BODY()
public:
	//老鼠的所有回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMouseRound> M_MouseRound;
};
