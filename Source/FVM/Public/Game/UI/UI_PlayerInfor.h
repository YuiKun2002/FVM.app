// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_PlayerInfor.generated.h"

/**
 *
 */

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class FVM_API UUI_PlayerInfor : public UWidgetBase
{
	GENERATED_BODY()
private:
	//头像
	UPROPERTY()
		UImage* M_PlayerHead = nullptr;
	//等级
	UPROPERTY()
		UImage* M_PlayerGrade = nullptr;

	//进度条图片
	// 
	//主要的控制
	UPROPERTY()
		UImage* M_Player_Progress_Main = nullptr;
	//背景显示
	UPROPERTY()
		UImage* M_Player_Progress_Bg = nullptr;
	//显示进度文字
	UPROPERTY()
		UTextBlock* M_Player_Progress_Text = nullptr;

	//显示进度文字
	UPROPERTY()
		UButton* M_Player_VIP = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerTip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerLog;
public:
	virtual bool Initialize() override;
public:
	//更新名称
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerInfor();
	//更新等级
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerGrade();
	//更新头像
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerHead();
	//更新经验值
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerEx();
};
