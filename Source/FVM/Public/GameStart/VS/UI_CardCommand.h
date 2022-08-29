// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_CardCommand.generated.h"

class UUI_MapMeshe;
class ACreateCardBar;
class UCardManagerComponent;

UCLASS()
class FVM_API UUI_CardCommand : public UWidgetBase
{
	GENERATED_BODY()
public:
	//����
	UUI_MapMeshe* M_MapMeshe = nullptr;
public:
	//����3D�����Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACreateCardBar* M_ACreateCardBar = nullptr;
public:
	virtual bool Initialize() override;
	//��ʼ��
	void Init();
public:
	//�������õĿ�Ƭ
	UFUNCTION(BlueprintCallable)
		void EradicateCard();
	//�����¿�Ƭ
	UFUNCTION(BlueprintCallable)
		void CreateNewCard(UCardManagerComponent* _CardComp);
	//�Ƴ���ǰUI
	UFUNCTION(BlueprintCallable)
		void RemoveCurrentPanel();
};
