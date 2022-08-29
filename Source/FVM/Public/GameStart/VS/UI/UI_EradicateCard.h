// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_EradicateCard.generated.h"

/**
 * ²ù³ý¿¨Æ¬
 */

class UButton;

UCLASS()
class FVM_API UUI_EradicateCard : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UButton* M_Eradicate = nullptr;

public:
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable)
		UButton* GetButton();
};
