// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStart/VS/MapBaseType.h"
#include "MapBase.generated.h"

UCLASS()
class FVM_API AMapBase : public AActor
{
	GENERATED_BODY()
protected:
	//��ͼλ��
	UPROPERTY(EditAnywhere, Category = "MapBaseStructData")
		FMapTranform M_MapTranform;

	//��ͼ��Ҫ�ľ�����Դ
	UPROPERTY(EditAnywhere, Category = "MapBaseStructData")
		FMapSprite M_MapSprite;

private:
	//��ʼ��������Դ
	void InitMapSprite();

public:
	// Sets default values for this actor's properties
	AMapBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
