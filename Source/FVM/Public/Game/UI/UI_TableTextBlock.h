// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_TableTextBlock.generated.h"

/**
 *
 */
UCLASS()
class FVM_API UUI_TableTextBlock : public UWidgetBase
{
	GENERATED_BODY()
public:
	//БъЬт
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_Title;
public:
	virtual bool Initialize() override;
public:
	void SetText(const FString& _Content);
};
