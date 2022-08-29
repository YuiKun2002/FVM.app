// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "GameSystem/Item/ItemStruct.h"
#include "MaterialDataStruct.generated.h"

/**
 *
 */

 //����-�������ϳ��䷽����
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisBlueprint_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//�䷽�ṹ
	UPROPERTY(EditDefaultsOnly)
		FCardBlueprint M_FMaterial;
};

//����-�������ϳ��䷽��������
USTRUCT(BlueprintType)
struct FMaterial_CardSynthesisMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//�䷽���Ͻṹ
	UPROPERTY(EditDefaultsOnly)
		FCardBlueprintMaterial M_FMaterial;
};

//����-������תְ��������
USTRUCT(BlueprintType)
struct FMaterial_CardChangeJobMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//תְ���Ͻṹ
	UPROPERTY(EditDefaultsOnly)
		FCardChangeJobMaterial M_FMaterial;
};

//����-��������
USTRUCT(BlueprintType)
struct FMaterial_SpicesMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//תְ���Ͻṹ
	UPROPERTY(EditDefaultsOnly)
		FSpicesMaterial M_FMaterial;
};

//����-��Ҷ������
USTRUCT(BlueprintType)
struct FMaterial_CloverMaterial_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//��Ҷ�ݽṹ
	UPROPERTY(EditDefaultsOnly)
		FCloverMaterial M_FMaterial;
};

UCLASS()
class FVM_API UMaterialDataStruct : public UObject
{
	GENERATED_BODY()

};
