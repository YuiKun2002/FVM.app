// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/PlayerStructManager.h"
#include "UI_PlayerBagEquipmentGrid.generated.h"

/**
 *
 */

 /*
 ����ṹ���ڽ��ձ���UI�д�����������->�����ǰ��Ʒ�Ǳ�����ô�ͻ�ʹ�õ��������
 */
 //��ɫ�����ṹ
struct FPlayerBagData {
public:
	//����
	FString VectorName;
	//��������
	uint8 M_PanelNumber;
	//�ڼ�������
	uint8 M_GridNumber;
};

class UButton;
class UUI_PlayerBag;

UCLASS()
class FVM_API UUI_PlayerBagEquipmentGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//���߰�ť->��������
	UPROPERTY()
		UButton* M_UButton = nullptr;
private:
	FString M_ItemName;
public:
	//ָ�򱳰�������
	FEquipmentBase* M_EuipmentData = nullptr;
	//����UI
	UUI_PlayerBag* M_BagUI = nullptr;
	//��������
	FPlayerBagData M_FPlayerBagData;
public:
	//������ʾ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemCountText;
public:
	virtual bool Initialize() override;
	//���¼��ر���װ����
	UFUNCTION()
		void RelaodBagEquipment();
	//��������
	UFUNCTION()
		void AttachToBag();
	//����һ���µ������ʾ����
	UFUNCTION()
		void CreateNewGiftBox();
	//��������
	UFUNCTION()
		void SetItemName(const FString& _Name);
	//��ȡ����
	UFUNCTION()
		FString GetItemName();
public:
	//��ȡ��ť
	UButton* GetButton();
};
