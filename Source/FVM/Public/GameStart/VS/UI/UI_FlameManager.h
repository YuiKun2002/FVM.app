// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_FlameManager.generated.h"

class UTextBlock;

UCLASS()
class FVM_API UUI_FlameManager : public UWidgetBase
{
	GENERATED_BODY()
private:
	//显示的UI文本
	UPROPERTY(EditAnywhere)
		UTextBlock* M_FlameNumText = nullptr;
	//火苗数量
	UPROPERTY(EditAnywhere)
		int32 M_FlameNum = 0;
public:
	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//获取火苗数量
	UFUNCTION(BlueprintCallable)
		int32 GetFlameNum();
};
