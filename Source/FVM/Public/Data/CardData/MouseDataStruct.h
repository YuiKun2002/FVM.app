// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/MouseStructManager.h"
#include "MouseDataStruct.generated.h"

/**
 *
 */

 //½��������ṹ
USTRUCT(BlueprintType)
struct FMouse_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//½�����͵Ŀ�Ƭ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FMouseBase M_Mouse;
};


UCLASS()
class FVM_API UMouseDataStruct : public UObject
{
	GENERATED_BODY()

};
