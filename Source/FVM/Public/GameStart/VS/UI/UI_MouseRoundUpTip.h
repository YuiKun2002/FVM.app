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
	//����ʱ�ı�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_Time_Text = nullptr;

	//Կ�׵�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_Key_Name = nullptr;

	//Կ�׵�ͼƬ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* M_Key_Image = nullptr;
public:
	virtual bool Initialize() override;
public:
	//����Կ��ͼƬ
	UFUNCTION(BlueprintCallable)
		void SetKeyImage(const FString& _ImageName);
	//����Կ������
	UFUNCTION(BlueprintCallable)
		void SetKeyName(const FString& _KeyName);
	//��Ϸ����
	UFUNCTION(BlueprintCallable)
		void GameWin();
	//��Ϸ����
	UFUNCTION(BlueprintCallable)
		void GameContinue();
};
