// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorDestroy.generated.h"

class UActorComponent;

UCLASS()
class FVM_API UActorDestroy : public UObject
{
	GENERATED_BODY()
public:

	//判断Actor是否存在?
	UFUNCTION(BlueprintCallable)
		static bool DoesActorExist(AActor* _Actor);

	//判断Actor组件是否存在?
	UFUNCTION(BlueprintCallable)
		static bool DoesActorComponentExist(UActorComponent* _ActorC);
};
