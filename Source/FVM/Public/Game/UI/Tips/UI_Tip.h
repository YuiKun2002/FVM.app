// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Tip.generated.h"

/**
 *
 */

class UTextBlock;

UCLASS()
class FVM_API UUI_Tip : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UTextBlock* M_Text = nullptr;
public:
	//��ʾ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ContentTips;
public:
	//������ʾ����
	UFUNCTION(BlueprintCallable)
		void SetTipText(const FString& _Text);
	//�����ı���ɫ
	UFUNCTION(BlueprintCallable)
		void SetTextColor(const FVector& _Vec3,float Alpha);
public:
	virtual bool Initialize() override;
};
