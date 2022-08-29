// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/MouseStructManager.h"
#include "MouseManagerComponent.generated.h"

class UFVMGameInstance;
class UGameMapStructManager;
class UMouseStructManager;
class UFVMGameInstance;
class AMouseActor;
class UMesheControllComponent;

class UUI_MouseRound;
class UUI_MouseBoss_Slider;

//老鼠线路通知
USTRUCT(BlueprintType)
struct FMouseLineNotify {
	GENERATED_USTRUCT_BODY()
public:
	//当前线路的坐标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_CurrentLineLocation = FVector::ZeroVector;
	//第几条线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Line = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//存放每条路线的通知器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMouseLineNotify> M_FMouseLineNotify;
	//老鼠管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMouseStructManager* M_UMouseStructManager = nullptr;
	//老鼠回合和Boss的血量UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_MouseRound* M_UUI_MouseRound = nullptr;
	//当前是第几回合
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CurrentRound = 0;
	//当前回合生成老鼠的索引(对应 M_MouseLineSetting[来自于回合结构体])
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CurrentSpawnMouseIndex = 0;
	//当前回合生成了多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentRoundSpawnMouseCount = 0;
	//当前回合一共会生成多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentRoundMouseMax = 0;
	//一共存活了多少只老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CurrentExistMouseCount = 0;

	//老鼠回合开始定时器
	FTimerHandle M_TimeHandle_MouseRounBegin;
	//老鼠回合结束定时器
	FTimerHandle M_TimeHandle_MouseRounEnd;
	//自动进入下一波定时器
	FTimerHandle M_TimeHandle_AutoInMouseRound;
	//自动生成老鼠的定时器
	FTimerHandle M_TimeHandle_AutoSpawnMouse;
public:
	// Sets default values for this component's properties
	UMouseManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//初始化线路
	void InitMouseLineNotify();
	//初始化老鼠配置
	void InitMouseStructManager();
	//初始化老鼠回合定时器
	void InitMouseRoundTime();
	//清除所有的定时器
	void ClearAllTimeHandle();
protected:
	//老鼠波数开始
	UFUNCTION()
		void MouseRoundBegin();
	//老鼠波数结束
	UFUNCTION()
		void MouseRoundEnd();
	//进入下一个回合
	UFUNCTION()
		void InNextRound();
	//生成老鼠
	UFUNCTION()
		void SpawnMouse();
	//设置老鼠的基本属性已经更新
	UFUNCTION()
		void MouseAttrSetting(AMouseActor* _CurrentMouse, TArray<float>& _Offset, TArray<float>& _OffsetLayer, const int32& _Line = 0);
	//创建一个老鼠
	UFUNCTION()
		AMouseActor* CreateNewMouse(const FString& _MouseName, const FVector& _Location = FVector::ZeroVector, const FRotator& _Rotation = FRotator(0.f, 90.f, 0.f), const FVector& _Scale = FVector(1.f));
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//打印老鼠线路通知结构体
	UFUNCTION(BlueprintCallable)
		void PrintMouseLineNotify(const FMouseLineNotify& _FMouseLineNotify);
	//显示UI
	UFUNCTION(BlueprintCallable)
		void ShowMouseUI();
	//移除UI
	UFUNCTION(BlueprintCallable)
		void RemoveMouseUI();
	//回合开始初始化
	UFUNCTION(BlueprintCallable)
		void InitNextRound();
	//是否是最后一个回合
	UFUNCTION(BlueprintCallable)
		bool IsEndRound();
	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
