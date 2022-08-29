// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MouseRound.generated.h"

class USlider;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UVerticalBox;

UCLASS()
class FVM_API UUI_MouseRound : public UWidgetBase
{
	GENERATED_BODY()
public:
	//���������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USlider* M_USlider = nullptr;
	//��������غ�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_MouseRoundText = nullptr;
	//boss���б�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVerticalBox* M_Boss_List_VerticalBox = nullptr;

public:
	virtual bool Initialize() override;

	//�������������
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundSlider(const float& _Curr, const float& _End);

	//��������غ�����
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundText(const int32& _Round);

	//��boss��Ѫ��������ӵ��б���
	UFUNCTION(BlueprintCallable)
		void AddMouseBossSlider(UWidget* _Widget_);
};
