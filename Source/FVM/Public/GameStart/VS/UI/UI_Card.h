// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Card.generated.h"

class ACardCoolDownManager;
class UCardManagerComponent;
class UImage;

UCLASS()
class FVM_API UUI_Card : public UWidgetBase
{
	GENERATED_BODY()
public:

	//卡片索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardIndex = -1;

	//卡片名称(该名称用于文件资源名称)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Card_ActorName;

	//需要的火苗数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_NeedFlame = 250;

	//卡片背景
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_UImage = nullptr;

	//卡片等级图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_UImage_Grade = nullptr;

public:
	virtual bool Initialize() override;
public:
	//获取冷却时间
	UFUNCTION(BlueprintCallable)
		bool GetCoolDownFinish();
	//获取完成比率
	UFUNCTION(BlueprintCallable)
		float GetCoolDownFinishRate();
	//设置卡片等级
	UFUNCTION(BlueprintCallable)
		void SetCardGrade(const int32& _Grade);
	//获取背景
	UFUNCTION(BlueprintCallable)
		UImage* GetUImage();
	//选中时的颜色
	UFUNCTION(BlueprintCallable)
		void SelectColor();
	//取消时的颜色
	UFUNCTION(BlueprintCallable)
		void CancelColor();
};
