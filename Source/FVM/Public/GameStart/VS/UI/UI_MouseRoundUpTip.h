// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MouseRoundUpTip.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class FVM_API UUI_MouseRoundUpTip : public UWidgetBase
{
	GENERATED_BODY()
public:
	//倒计时文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_Time_Text = nullptr;

	//钥匙的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_Key_Name = nullptr;

	//钥匙的图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_Key_Image = nullptr;
public:
	virtual bool Initialize() override;
public:
	//设置钥匙图片
	UFUNCTION(BlueprintCallable)
		void SetKeyImage(const FString& _ImageName);
	//设置钥匙名称
	UFUNCTION(BlueprintCallable)
		void SetKeyName(const FString& _KeyName);
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameWin();
	//游戏继续
	UFUNCTION(BlueprintCallable)
		void GameContinue();
};
