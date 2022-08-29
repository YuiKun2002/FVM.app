// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VSManagerComponent.generated.h"

class UUI_GameOver;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UVSManagerComponent : public UActorComponent
{
	GENERATED_BODY()
private:

	//游戏暂停
	UPROPERTY(EditAnywhere)
		bool M_bGamePause = false;
	//游戏结束UI
	UPROPERTY(EditAnywhere)
		UUI_GameOver* M_UI_GameOver = nullptr;
public:
	//是否是胜利
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		bool M_Win = true;
public:
	// Sets default values for this component's properties
	UVSManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//游戏是否暂停
	UFUNCTION(BlueprintCallable)
		bool IsGamePause();

	//游戏暂停
	UFUNCTION(BlueprintCallable)
		void GamePause();

	//游戏继续
	UFUNCTION(BlueprintCallable)
		void GameContinue();

	//开始游戏
	UFUNCTION(BlueprintCallable)
		void GameStart();

	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
