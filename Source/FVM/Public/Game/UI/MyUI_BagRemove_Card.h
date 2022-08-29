// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/UI/UI_BagRemoveItemBase.h"
#include "MyUI_BagRemove_Card.generated.h"


class UImage;

UCLASS()
class FVM_API UMyUI_BagRemove_Card : public UUI_BagRemoveItemBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UImage* M_CardGradeImage = nullptr;
	UPROPERTY()
		UImage* M_CardHead = nullptr;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		FString M_CardName;
public:
	virtual bool Initialize() override;
	//设置卡片
	void SetCard(const FString& _HeadPath, int32 _CardGrade);
};
