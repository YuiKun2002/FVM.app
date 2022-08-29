// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/FlipbookActorBase.h"
#include "FlameActor.generated.h"

/**
 * 火苗 用于在游戏中提供购买卡片的资源   一般通过资源管理器生成或者对应卡片生成
 */
UCLASS()
class FVM_API AFlameActor : public AFlipbookActorBase
{
	GENERATED_BODY()
private:
	//基础资源值
	UPROPERTY(EditAnywhere)
		int32 M_Value = 25;

	//存活时间
	UPROPERTY(EditAnywhere)
		float M_LifeTime = 15.f;

	//当前的时间
	UPROPERTY(EditAnywhere)
		float M_CurrentTime = 0.f;
public:

	AFlameActor();

	//设置存活时间
	UFUNCTION(BlueprintCallable)
		void SetFlameTime(const float& _time);

	//设置资源值
	UFUNCTION(BlueprintCallable)
		void SetFlameValue(const int32& _value);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;


};
