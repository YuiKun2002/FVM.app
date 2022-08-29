// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/UI/UI_BagRemoveItemBase.h"
#include "UI_BagRemove_Equipment.generated.h"

/**
 *
 */

class UImage;
class UHorizontalBox;

UCLASS()
class FVM_API UUI_BagRemove_Equipment : public UUI_BagRemoveItemBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UImage* M_EquipmentHead = nullptr;
	UPROPERTY()
		UHorizontalBox* M_EquipmentCountPanel = nullptr;
public:
	//装备名称
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_EquipmentName;
	//装备数量
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_EquipmentNum;
public:
	virtual bool Initialize() override;

	//设置装备
	void SetEquipment(const FString& _HeadPath,bool _IsOverlap,int32 _ItemCount);
};
