// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_CardBar.generated.h"

class UHorizontalBox;

UCLASS()
class FVM_API UUI_CardBar : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		UHorizontalBox* M_UHorizontalBox = nullptr;
public:
	//获取卡片列表
	UFUNCTION(BlueprintCallable)
		UHorizontalBox* GetCardList();
public:
	virtual bool Initialize() override;
};
