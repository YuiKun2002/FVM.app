// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_CardCommand.generated.h"

class UUI_MapMeshe;
class ACreateCardBar;
class UCardManagerComponent;

UCLASS()
class FVM_API UUI_CardCommand : public UWidgetBase
{
	GENERATED_BODY()
public:
	//网格
	UUI_MapMeshe* M_MapMeshe = nullptr;
public:
	//创建3D界面的Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACreateCardBar* M_ACreateCardBar = nullptr;
public:
	virtual bool Initialize() override;
	//初始化
	void Init();
public:
	//铲除放置的卡片
	UFUNCTION(BlueprintCallable)
		void EradicateCard();
	//创建新卡片
	UFUNCTION(BlueprintCallable)
		void CreateNewCard(UCardManagerComponent* _CardComp);
	//移除当前UI
	UFUNCTION(BlueprintCallable)
		void RemoveCurrentPanel();
};
