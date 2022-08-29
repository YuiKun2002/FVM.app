// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceManagerComponent.generated.h"

class AFlameActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UResourceManagerComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//自动生成火苗的计时器
	FTimerHandle M_TimerHandleAutoSpawnFlame;
public:

	/*---------------------------------火苗---------------------------------*/
	//火苗默认数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_FlameNum = 500;
	/*---------------------------------火苗---------------------------------*/


	//是否是白天
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDayTime = true;
public:
	// Sets default values for this component's properties
	UResourceManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*---------------------------------火苗---------------------------------*/
	//开启火苗自动掉落
	void EnableAutoFlameSpawn(const float& _SpawnTime);
	//添加火苗
	UFUNCTION(BlueprintCallable)
		void AddFlameNum(const int32& _Value);
	//减少火苗
	UFUNCTION(BlueprintCallable)
		void SubtractFlameNum(const int32& _Value);
	//获取火苗
	UFUNCTION(BlueprintCallable)
		const int32& GetCurrentFlameNum();
	/*---------------------------------火苗---------------------------------*/

	/*---------------------------------天---------------------------------*/

	//获取是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetDay();
	/*---------------------------------天---------------------------------*/

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();

	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
