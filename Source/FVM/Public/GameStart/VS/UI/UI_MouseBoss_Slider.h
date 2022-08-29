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
	//boss����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCanvasPanel* M_BossCanvasPanel = nullptr;
	//boss����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_BossNameText = nullptr;
	//bossͷ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_BossHead = nullptr;
	//������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_Slider_Image = nullptr;
public:
	virtual bool Initialize() override;
public:
	//���ý����������
	UFUNCTION(BlueprintCallable)
		void SetSliderRate(float Value);
	//����boss����
	UFUNCTION(BlueprintCallable)
		void SetBossName(const FString& _Name);
	//����boss��ͷ��->����ͷ������
	UFUNCTION(BlueprintCallable)
		void SetBossHead(const FString& _HeadName);
};
