// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include <Components/Button.h>

#include "GameSystem/Tools/GameSystemFunction.h"
#include "Data/MaterialDataStruct.h"
#include "Game/UI/UI_PlayerSynthesis.h"

bool UUI_PlayerBagMaterialGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Button = this->GetWidgetComponent<UButton>(this, "Item_button");

	return true;
}

void UUI_PlayerBagMaterialGrid::SetUI_PlayerSynthesis(UUI_PlayerSynthesis* _UUI_PlayerSynthesis)
{
	this->M_UUI_PlayerSynthesis = _UUI_PlayerSynthesis;

	//合成屋香料选择
	if (this->M_MaterialBaseData && this->M_MaterialBaseData->M_MaterialType == EMaterialType::E_Spices && !this->M_UUI_PlayerSynthesis->M_MakeCard_SpicesGrid)
	{
		if (this->M_MaterialBaseData->ItemName.Equals(this->M_UUI_PlayerSynthesis->M_SelectSpicesName))
		{
			this->AddSynthesisSpicesSlot();
		}
	}
}

inline void UUI_PlayerBagMaterialGrid::SetMaterialData(FMaterialBase* _Data)
{
	this->M_MaterialBaseData = _Data;
}

FMaterialBase* const UUI_PlayerBagMaterialGrid::GetMaterialData()
{
	return this->M_MaterialBaseData;
}

UButton* UUI_PlayerBagMaterialGrid::GetButton()
{
	return this->M_Button;
}

void UUI_PlayerBagMaterialGrid::UpdateMaterialsShowCount(FString  _CountText)
{
	this->M_CountText = *_CountText;
}

FMaterialBase* GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName)
{
	for (FMaterialBase& Items : _Arrays)
	{
		if (Items.ItemName.Equals(_ItemName))
			return &Items;
	}
	return nullptr;
}


//图片设置 给UUI_PlayerBagMaterialGrid::AddSynthesisBlueprint使用的专属函数
inline void SetImageGreyColor(FMaterialBase* _Item, UImage* _Image, const FString& _Path)
{
	if (_Item == nullptr)
		UWidgetBase::SetImageBrush(_Image, _Path, FVector(0.3f, 0.3f, 0.3f));
	else
		UWidgetBase::SetImageBrush(_Image, _Path);

}

void UUI_PlayerBagMaterialGrid::AddSynthesisBlueprint()
{
	this->M_UUI_PlayerSynthesis->M_MaterialsData.Empty();
	//原因:它指向的玩家背包中加载的数据->并非新创建的数据，被指向的数据由玩家角色结构管理器统一管理
	this->M_UUI_PlayerSynthesis->M_MaterialBlueprintData = nullptr;

	TArray<FMaterial_CardSynthesisBlueprint_Data*> SynthesisBlueprintDatas;
	TArray<FMaterial_CardSynthesisMaterial_Data*> SynthesisDatas;
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisBlueprint_Data>("DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisBlueprint_Data.Material_CardSynthesisBlueprint_Data'", SynthesisBlueprintDatas);
	UGameSystemFunction::GetDataTableRows<FMaterial_CardSynthesisMaterial_Data>("DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisMaterial_Data.Material_CardSynthesisMaterial_Data'", SynthesisDatas);

	//卡片数据
	FCardBlueprint* CardBlueprintData = nullptr;

	//查询卡片数据
	for (auto Items : SynthesisBlueprintDatas)
	{
		if (Items->M_FMaterial.ItemName.Equals(this->M_FMaterialBaseData.ItemName))
		{
			CardBlueprintData = &Items->M_FMaterial;
			break;
		}
	}

	//对应材料的图片路径
	TMap<FString, FString> M_MaterialTextures;

	TArray<FString> M_MaterialsName = CardBlueprintData->M_Materials;

	//查询材料纹理数据
	for (auto Items : SynthesisDatas)
	{
		int32 _Index = 0;
		for (auto MaterialName : M_MaterialsName)
		{
			if (Items->M_FMaterial.ItemName.Equals(MaterialName))
			{
				M_MaterialTextures.Emplace(Items->M_FMaterial.ItemName, Items->M_FMaterial.ItemTexturePath);
				M_MaterialsName.RemoveAt(_Index);
				break;
			}
			_Index++;
		}
	}

	//根据卡片数据从角色背包寻找对应材料
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//获取对应数据(配方材料)
	for (auto ItemName : CardBlueprintData->M_Materials)
	{
		this->M_UUI_PlayerSynthesis->M_MaterialsData.Emplace(UGameSystemFunction::GetMaterialDataFromBag(PlayerData->M_PlayerItems_Material, ItemName));
	}
	//获取配方名称
	this->M_UUI_PlayerSynthesis->M_MaterialBlueprintName = this->M_FMaterialBaseData.ItemName;
	//获取对应数据(合成配方)
	this->M_UUI_PlayerSynthesis->M_MaterialBlueprintData = UGameSystemFunction::GetMaterialDataFromBag(PlayerData->M_PlayerItems_Material, this->M_FMaterialBaseData.ItemName);
	//获取最终合成的卡片名称
	this->M_UUI_PlayerSynthesis->M_MaterialBlueprintTargetName = CardBlueprintData->M_SynthesisCardName;

	//更新图片显示样式
	UWidgetBase::SetButtonStyle(this->M_UUI_PlayerSynthesis->M_SynthesisBlueprint_Butt, CardBlueprintData->ItemTexturePath);

	//颜色判断->如果玩家没有这个道具那么显示的时候颜色暗淡 0.5f
	SetImageGreyColor(this->M_UUI_PlayerSynthesis->M_MaterialsData[0], this->M_UUI_PlayerSynthesis->M_Material_0_Image, *M_MaterialTextures.Find(CardBlueprintData->M_Materials[0]));
	SetImageGreyColor(this->M_UUI_PlayerSynthesis->M_MaterialsData[1], this->M_UUI_PlayerSynthesis->M_Material_1_Image, *M_MaterialTextures.Find(CardBlueprintData->M_Materials[1]));
	SetImageGreyColor(this->M_UUI_PlayerSynthesis->M_MaterialsData[2], this->M_UUI_PlayerSynthesis->M_Material_2_Image, *M_MaterialTextures.Find(CardBlueprintData->M_Materials[2]));
}

void UUI_PlayerBagMaterialGrid::AddSynthesisSpicesSlot()
{
	//判断当前香料有没有达到5个
	if (this->M_MaterialBaseData->M_Count < 5)
	{
		UWidgetBase::CreateTipWidget(TEXT("香料个数需要达到5个才能添加哦"));
		return;
	}


	if (this->M_UUI_PlayerSynthesis->M_MakeCard_SpicesGrid)
	{
		this->M_UUI_PlayerSynthesis->M_MakeCard_SpicesGrid->GetButton()->SetIsEnabled(true);
	}

	this->M_UUI_PlayerSynthesis->M_MakeCard_SpicesGrid = this;
	this->GetButton()->SetIsEnabled(false);

	//设置按钮样式
	UWidgetBase::SetButtonStyle(this->M_UUI_PlayerSynthesis->M_Spices_Butt, this->M_MaterialBaseData->ItemTexturePath);
}

void UUI_PlayerBagMaterialGrid::AddUpGradeCardCloverSlot()
{
	if (this->M_UUI_PlayerSynthesis->M_MakeCard_CloverGrid)
	{
		this->M_UUI_PlayerSynthesis->M_MakeCard_CloverGrid->GetButton()->SetIsEnabled(true);
	}

	this->M_UUI_PlayerSynthesis->M_MakeCard_CloverGrid = this;
	this->GetButton()->SetIsEnabled(false);

	//设置按钮样式
	UWidgetBase::SetButtonStyle(this->M_UUI_PlayerSynthesis->M_Clover_Butt, this->M_MaterialBaseData->ItemTexturePath);
	//更新概率
	this->M_UUI_PlayerSynthesis->GetUpGradeRate();
}


