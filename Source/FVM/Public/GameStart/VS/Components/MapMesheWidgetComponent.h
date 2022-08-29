// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "MapMesheWidgetComponent.generated.h"

class UUI_MapMeshe;
class AMapMeshe;

UCLASS()
class FVM_API UMapMesheWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
};
