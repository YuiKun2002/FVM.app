// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MouseBoss_Slider.generated.h"

class USlider;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UCanvasPanelSlot;

UCLASS()
class FVM_API UUI_MouseBoss_Slider : public UWidgetBase
{
	GENERATED_BODY()
public:
	//boss界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvasPanel* M_BossCanvasPanel = nullptr;
	//boss名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_BossNameText = nullptr;
	//boss头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BossHead = nullptr;
	//进度条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_Slider_Image = nullptr;
public:
	virtual bool Initialize() override;
public:
	//设置进度条完成率
	UFUNCTION(BlueprintCallable)
		void SetSliderRate(float Value);
	//设置boss名称
	UFUNCTION(BlueprintCallable)
		void SetBossName(const FString& _Name);
	//设置boss的头像->传入头像名称
	UFUNCTION(BlueprintCallable)
		void SetBossHead(const FString& _HeadName);
};
