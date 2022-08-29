// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Classes/Engine/DataTable.h"
#include "ShopItemPriceStruct.generated.h"

class UPlayerStructManager;
class UGameConfigManager;
class UFVMGameInstance;

//物品类型
UENUM(BlueprintType)
enum class EItemType : uint8 {
	//卡片
	E_Card UMETA(DisplayName = "Card"),
	//装备
	E_Equipment UMETA(DisplayName = "Equipment"),
	//材料
	E_MaterialItem UMETA(DisplayName = "Material_Item"),


	//特殊的支付对象(背包格子用于扩充玩家背包放置个数)
	E_BagGrid UMETA(DisplayName = "BagGrid"),
};

//物品的结构
USTRUCT(BlueprintType)
struct FItemPrice {
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName;
	//简单的提示信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemTips;
	//物品的描述信息
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemDescirbe;
	//物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType M_ItemType;
	//物品的价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ItemPrice;
	//货币的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 M_ItemMoneyType;
	//货币的类型文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemMoneyTypeName;
	//展示的纹理图片路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemHeadTexture2DPath;
};

//商店物品的价格
USTRUCT(BlueprintType)
struct FItem_Price_Data : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();
public:
	//物品结构
	UPROPERTY(EditDefaultsOnly)
		FItemPrice M_FItemPrice;
};
 
UCLASS()
class FVM_API UShopItemPriceStruct : public UObject
{
	GENERATED_BODY()
public:
	static void AddToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);

	//开启背包界面第几个界面的第几个背包
	static void AddPlayerBagGrid(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Panel_Index);
private:
	//添加卡片到背包
	static void AddCardToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
	//添加装备到背包
	static void AddEquipmentToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
	//添加材料到背包
	static void AddMaterialToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count);
};
