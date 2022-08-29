// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStart/VS/MapBaseType.h"
#include "MapMeshe.generated.h"

class UMapMesheWidgetComponent;
class UWidgetComponent;
class UUI_MapMeshe;

UCLASS()
class FVM_API AMapMeshe : public AActor
{
	GENERATED_BODY()
private:
	FTransform _Trans;
public:
	//界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
		UMapMesheWidgetComponent* M_MapMesheWidgetComponent = nullptr;
	//当前线路
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_Line;
	//格子类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType;
public:
	// Sets default values for this actor's properties
	AMapMeshe();

	//设置路线
	UFUNCTION(BlueprintCallable)
		void SetLine(const int32& _Row, const int32& _Col, const ELineType _Type);
	//设置UI组件的位置
	UFUNCTION(BlueprintCallable)
		void SetWidgetComponentTransform(const FTransform& _ActorTransform);
	//'获取UI组件的位置
	UFUNCTION(BlueprintCallable)
		FTransform GetWidgetComponentTransform();
	//获取UImapmeshe
	UFUNCTION(BlueprintCallable)
		UUI_MapMeshe* GetUIMapMeshe();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
