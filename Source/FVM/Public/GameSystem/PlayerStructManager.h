// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Data/CardData/CardDataStruct.h"
#include "Data/EquipmentDataSturct.h"
#include "Data/MaterialDataStruct.h"
#include "PlayerStructManager.generated.h"

/**
 *  角色存储结构
 */

USTRUCT(BlueprintType)
struct FPlayerEx {
	GENERATED_USTRUCT_BODY()
public:
	//当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentEx = 0.f;
	//当前经验值顶峰
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentExTop = 100.f;
	//经验值成长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ExUp = 10.f;
	//经验值成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ExUpRate = 0.4f;
	//最大经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ExMax = 1000000.f;
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_PlayerGrade = 1;
};

USTRUCT(BlueprintType)
struct FPlayerVIP
{
	GENERATED_USTRUCT_BODY()
public:
	//当前VIP值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_VIP = 0;
	//最大VIP
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 M_MaxVIP = 12;
	//当前进入VIP经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentEx = 0.f;
	//当前VIP最大经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentExTop = 100.f;
	//VIP成长值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_VIP_Up = 2.f;
	//VIP成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_VIP_UpRate = 0.4f;
};

USTRUCT(BlueprintType)
struct FPlayerSign
{
	GENERATED_USTRUCT_BODY()
public:
	//角色等级称号
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerFlag;
	//角色描述QQ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_QQ;
};

//角色货币(存储信息)
USTRUCT(BlueprintType)
struct FPlayerCoin {
	GENERATED_USTRUCT_BODY()
public:
	//货币分等级

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Coin_0 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Coin_1 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Coin_2 = 0;
};

//角色背包的格子
USTRUCT(BlueprintType)
struct FPlayerBagGird
{
	GENERATED_USTRUCT_BODY()
public:
	//背景图片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_GridImagePath;
	//容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GridCount;
	//第几个界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_PanelNumber;
	//第几个背包
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_BagNumber;
	//Vector名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_VectorName;
	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_BagName;
};


//角色背包的格子组合(武器 防御卡 材料)
USTRUCT(BlueprintType)
struct FPlayerBagGirdGroup
{
	GENERATED_USTRUCT_BODY()
public:
	//默认容量150
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 M_DefGridCount = 150;
	//装备背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerBagGird> M_EquipmentGrid;
	//卡片背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerBagGird> M_CardGrid;
	//材料背包格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FPlayerBagGird> M_MaterialGrid;
private:
	int32 GetBagCount(TArray<FPlayerBagGird>& _Arrays)
	{
		int32 _LCount = 150;
		for (FPlayerBagGird& _bag : _Arrays)
		{
			_LCount += _bag.M_GridCount;
		}
		return _LCount;
	}
public:

	//获取背包容量（0表示获取装备背包 1表示卡片背包 2表示材料背包）
	int32 GetBagCount(uint8 _Index)
	{
		switch (_Index)
		{
			//卡片
		case 1:return GetBagCount(this->M_CardGrid);
			//材料
		case 2:return GetBagCount(this->M_MaterialGrid);
			//装备
		default:return GetBagCount(this->M_EquipmentGrid);
		}
	}

	TArray<FPlayerBagGird*> GetUseBagArrays()
	{
		TArray<FPlayerBagGird*> Items;

		for (FPlayerBagGird& Equipment : this->M_EquipmentGrid)
			if (!Equipment.M_VectorName.Equals(""))
				Items.Emplace(&Equipment);

		for (FPlayerBagGird& Card : this->M_CardGrid)
			if (!Card.M_VectorName.Equals(""))
				Items.Emplace(&Card);

		for (FPlayerBagGird& Material : this->M_MaterialGrid)
			if (!Material.M_VectorName.Equals(""))
				Items.Emplace(&Material);

		return Items;
	}

	static void ClearData(FPlayerBagGird& _Grid)
	{
		//背景图片
		_Grid.M_GridImagePath = "";
		//容量
		_Grid.M_GridCount = 0;
		//第几个界面
		_Grid.M_PanelNumber = -1;
		//第几个背包
		_Grid.M_BagNumber = -1;
		//Vector名称
		_Grid.M_VectorName = "";
		//道具名称
		_Grid.M_BagName = "";
	}
};


UCLASS()
class FVM_API UPlayerStructManager : public USaveGame
{
	GENERATED_BODY()
public:
	//角色版本
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_GameVersion = "";
	// 0是女生  1是男生
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 M_PlayerSex;
	//角色名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_PlayerName = FString(TEXT("面包干&芙蓉"));
	//角色等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerEx M_FPlayerEx;
	//角色货币
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerCoin M_FPlayerCoin;
	//角色VIP
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerVIP M_FPlayerVIP;
	//角色称号和描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerSign M_FPlayerSign;
	//背包界面中的背包容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FPlayerBagGirdGroup M_PlayerBagGirdGroup;
	//背包 _防御卡
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemCard> M_PlayerItems_Card;
	//背包_装备
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FEquipmentBase> M_PlayerItems_Equipment;
	//背包_材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMaterialBase> M_PlayerItems_Material;

public:

	//获取表数据
	template<class RowType>
	static void GetTableRow(const FString& _Path, TArray<RowType*>& _OutRows)
	{
		UDataTable* Table = LoadObject<UDataTable>(0, *_Path);
		Table->GetAllRows("List", _OutRows);
	}

	//从背包中寻找对应装备
	template <class ItemType = FMaterialBase>
	static ItemType* GetItemFromBag(TArray<ItemType>& _Player, const FString& _ItemName)
	{
		for (ItemType& Item : _Player)
		{
			if (Item.ItemName.Equals(_ItemName))
			{
				return &Item;
			}
		}

		return nullptr;
	}

	/*-----------------------------------------------材料-----------------------------------------------*/
	/*-----------------------------------------------材料-----------------------------------------------*/
	//创建一个新的材料数据

	static FMaterialBase CreateNewMaterial(const FString& _ItemName, EMaterialType _Type)
	{
		FMaterialBase Item;
		UPlayerStructManager::SearchMaterailFromDataTable(_ItemName, Item, true, _Type);
		return Item;
	}

	//获取指定材料通过数组
	template <class TargetType>
	void static GetMaterialArraysData(const FString& _Names, TArray<TargetType>& _Arrays, TargetType& _OutputData, bool& _Result)
	{
		//获取物品数据
		for (TargetType& Items : _Arrays)
			if (_Names.Equals(Items.ItemName))
			{
				_OutputData = Items;
				_Result = true;
				return;
			}
	}

	//获取指定数据的原结构
	template <class DataTableRowType, class MaterailType = FMaterialBase>
	static void GetMaterialSourceData(const FString& _Name, MaterailType& _Clover, EMaterialType _Type)
	{
		bool _Result;
		TArray<MaterailType> Items;
		UPlayerStructManager::GetMaterailRowDatas<DataTableRowType, MaterailType>(UPlayerStructManager::GetMaterialDataTablePath(_Type), Items);
		UPlayerStructManager::GetMaterialArraysData<MaterailType>(_Name, Items, _Clover, _Result);
	}

	//获取数据表所有数据
	template <class DataTableRowType, class MaterailType = FMaterialBase>
	static void GetMaterailRowDatas(const FString& _DataTableName, TArray<MaterailType>& _OutArrays, int32 _UniformCount = 1)
	{
		TArray<DataTableRowType*> _Item_Rows;
		UPlayerStructManager::GetTableRow<DataTableRowType>(_DataTableName, _Item_Rows);
		for (DataTableRowType* Item : _Item_Rows)
		{
			Item->M_FMaterial.M_Count = _UniformCount;
			_OutArrays.Emplace(Item->M_FMaterial);
		}
	}

	//返回所有材料
	template <class Type = FItemBase>
	static void GetAllMaterial(TArray<Type>& _Item, int32 _UniformCount = 1)
	{
		UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardSynthesisBlueprint_Data, Type>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Blueprint), _Item, _UniformCount);
		UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardSynthesisMaterial_Data, Type>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_CardSynthesisMaterial), _Item, _UniformCount);
		UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardChangeJobMaterial_Data, Type>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_CardChangeJobMaterial), _Item, _UniformCount);
		UPlayerStructManager::GetMaterailRowDatas<FMaterial_SpicesMaterial_Data, Type>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Spices), _Item, _UniformCount);
		UPlayerStructManager::GetMaterailRowDatas<FMaterial_CloverMaterial_Data, Type>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Clover), _Item, _UniformCount);
	}

	//根据类型返回数据表名称
	static FString GetMaterialDataTablePath(EMaterialType _Material)
	{
		switch (_Material)
		{
		case EMaterialType::E_Blueprint:return "DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisBlueprint_Data.Material_CardSynthesisBlueprint_Data'";
		case EMaterialType::E_CardSynthesisMaterial:return "DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisMaterial_Data.Material_CardSynthesisMaterial_Data'";
		case EMaterialType::E_CardChangeJobMaterial:return "DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardChangeJobMaterial_Data.Material_CardChangeJobMaterial_Data'";
		case EMaterialType::E_Spices:return "DataTable'/Game/Resource/BP/Data/MaterialData/Material_SpicesMaterial_Data.Material_SpicesMaterial_Data'";
		case EMaterialType::E_Clover:return "DataTable'/Game/Resource/BP/Data/MaterialData/Material_CloverMaterial_Data.Material_CloverMaterial_Data'";
		}
		return "";
	}

	//通过名称获取材料道具
	UFUNCTION(BlueprintCallable)
		static bool SearchMaterailFromDataTable(const FString& _MaterailName, FMaterialBase& OutputData, bool _SelectType = false, EMaterialType _Material = EMaterialType::E_Blueprint)
	{
		bool _Result = false;
		TArray<FMaterialBase> Items;
		if (!_SelectType || _SelectType && _Material == EMaterialType::E_Blueprint)
			UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardSynthesisBlueprint_Data, FMaterialBase>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Blueprint), Items);
		if (!_SelectType || _SelectType && _Material == EMaterialType::E_CardSynthesisMaterial)
			UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardSynthesisMaterial_Data, FMaterialBase>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_CardSynthesisMaterial), Items);
		if (!_SelectType || _SelectType && _Material == EMaterialType::E_CardChangeJobMaterial)
			UPlayerStructManager::GetMaterailRowDatas<FMaterial_CardChangeJobMaterial_Data, FMaterialBase>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_CardChangeJobMaterial), Items);
		if (!_SelectType || _SelectType && _Material == EMaterialType::E_Spices)
			UPlayerStructManager::GetMaterailRowDatas<FMaterial_SpicesMaterial_Data, FMaterialBase>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Spices), Items);
		if (!_SelectType || _SelectType && _Material == EMaterialType::E_Clover)
			UPlayerStructManager::GetMaterailRowDatas<FMaterial_CloverMaterial_Data, FMaterialBase>(UPlayerStructManager::GetMaterialDataTablePath(EMaterialType::E_Clover), Items);

		//查询
		UPlayerStructManager::GetMaterialArraysData<FMaterialBase>(_MaterailName, Items, OutputData, _Result);
		return _Result;
	}


	/*-----------------------------------------------材料-----------------------------------------------*/
	/*-----------------------------------------------材料-----------------------------------------------*/



	/*-----------------------------------------------装备-----------------------------------------------*/
	/*-----------------------------------------------装备-----------------------------------------------*/
	//获取指定装备通过数组
	template <class TargetType>
	void static GetEquipmentArraysData(const FString& _Names, TArray<TargetType>& _Arrays, TargetType& _OutputData, bool& _Result)
	{
		//获取物品数据
		for (TargetType& Items : _Arrays)
			if (_Names.Equals(Items.ItemName))
			{
				_OutputData = Items;
				_Result = true;
				return;
			}
	}
	//创建一个新的装备数据
	static FEquipmentBase CreateNewEquipment(const FString& _ItemName, EEquipment _Type)
	{
		FEquipmentBase Item;
		UPlayerStructManager::SearchEquipmentFromDataTable(_ItemName, Item, true, _Type);
		return Item;
	}
	//创建新的道具到背包
	UFUNCTION(BlueprintCallable)
		static void CreateNewEquipmentToPlayerBag(UPlayerStructManager* _Player, const FString& _ItemName, EEquipment _EquipmentType)
	{
		if (!_Player)
			return;

		FEquipmentBase BaseData = UPlayerStructManager::CreateNewEquipment(_ItemName, _EquipmentType);

		_Player->M_PlayerItems_Equipment.Emplace(BaseData);
	}

	//获取所有装备
	template <class DataTableRowType, class EquipmentType = FEquipmentBase>
	static void GetEquipmentRowDatas(const FString& _DataTableName, TArray<EquipmentType>& _OutArrays)
	{
		TArray<DataTableRowType*> _Item_Rows;
		UPlayerStructManager::GetTableRow<DataTableRowType>(_DataTableName, _Item_Rows);
		for (DataTableRowType* Item : _Item_Rows)
		{
			_OutArrays.Emplace(Item->M_FEquipment);
		}
	}

	//创建所有的装备到背包
	UFUNCTION(BlueprintCallable)
		static void CreateAllEquipmentToBag(UPlayerStructManager* _Player)
	{
		if (!_Player)
			return;

		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_Bag_Data, FEquipmentBase>(UPlayerStructManager::GetEquipmentDataTablePath(), _Player->M_PlayerItems_Equipment);
		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_PlayerEquipment_Data, FEquipmentBase>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_PlayerEquipment), _Player->M_PlayerItems_Equipment);
		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_GiftBox_Data, FEquipmentBase>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_Gift), _Player->M_PlayerItems_Equipment);
	}

	//获取装备数据表名称
	static FString GetEquipmentDataTablePath(EEquipment Type = EEquipment::E_Bag)
	{
		switch (Type)
		{
		case EEquipment::E_Bag:return "DataTable'/Game/Resource/BP/Data/EquipmentData/EquipmentBagData.EquipmentBagData'";
		case EEquipment::E_Gift:return "DataTable'/Game/Resource/BP/Data/EquipmentData/EquipmentGiftBoxData.EquipmentGiftBoxData'";
		case EEquipment::E_PlayerEquipment:return "DataTable'/Game/Resource/BP/Data/EquipmentData/EquipmentPlayerEquipmentData.EquipmentPlayerEquipmentData'";
		}
		return "";
	}

	//获取所有装备名称
	static void GetAllEquipmentName(TArray<FItemBase>& _Item)
	{
		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_Bag_Data, FItemBase>(UPlayerStructManager::GetEquipmentDataTablePath(), _Item);
		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_PlayerEquipment_Data, FItemBase>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_PlayerEquipment), _Item);
		UPlayerStructManager::GetEquipmentRowDatas<FEquipment_GiftBox_Data, FItemBase>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_Gift), _Item);
	}

	//通过名称获取装备数据
	UFUNCTION(BlueprintCallable)

		static bool SearchEquipmentFromDataTable(const FString& _EquipmentName, FEquipmentBase& OutputData, bool _bSelect = false, EEquipment Type = EEquipment::E_Bag)
	{
		TArray<FEquipmentBase> Item;

		bool _Result = false;

		if (!_bSelect || _bSelect && Type == EEquipment::E_Bag)
			UPlayerStructManager::GetEquipmentRowDatas<FEquipment_Bag_Data>(UPlayerStructManager::GetEquipmentDataTablePath(), Item);
		if (!_bSelect || _bSelect && Type == EEquipment::E_PlayerEquipment)
			UPlayerStructManager::GetEquipmentRowDatas<FEquipment_PlayerEquipment_Data>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_PlayerEquipment), Item);
		if (!_bSelect || _bSelect && Type == EEquipment::E_Gift)
			UPlayerStructManager::GetEquipmentRowDatas<FEquipment_GiftBox_Data>(UPlayerStructManager::GetEquipmentDataTablePath(EEquipment::E_Gift), Item);

		UPlayerStructManager::GetEquipmentArraysData<FEquipmentBase>(_EquipmentName, Item, OutputData, _Result);

		return _Result;
	}
	/*-----------------------------------------------装备-----------------------------------------------*/
	/*-----------------------------------------------装备-----------------------------------------------*/



	/*-----------------------------------------------卡片-----------------------------------------------*/
	/*-----------------------------------------------卡片-----------------------------------------------*/


	//根据类型返回数据表路径
	static FString GetCardDataTablePath(ECardType _Type)
	{
		switch (_Type)
		{
		case ECardType::E_ATK:
			return "DataTable'/Game/Resource/BP/Data/CardData/CardATKData.CardATKData'";
		case ECardType::E_SPAWN:
			return "DataTable'/Game/Resource/BP/Data/CardData/CardSpawnData.CardSpawnData'";
		case ECardType::E_DEFENCE:
			return "DataTable'/Game/Resource/BP/Data/CardData/CardDefenceData.CardDefenceData'";
		case ECardType::E_Aid:
			return "";
		case ECardType::E_Function:
			return "DataTable'/Game/Resource/BP/Data/CardData/CardFunctionData.CardFunctionData'";
		}
		return "";
	}

	//卡片名称匹配
	template<class Type, class OutType = FItemCard>
	static void  GetCardArraysData(const FString& _CardName, TArray<Type>& _Arrays, OutType& OutputData, bool& _Result)
	{
		//查询卡片
		for (Type& _Card : _Arrays)
			if (_Card.ItemName.Equals(_CardName))
			{
				OutputData = _Card;
				_Result = true;
			}
	}

	//返回卡片数据数组
	template <class DataTableRowType>
	static void GetCardName(const FString& _DataTablePath, TArray<FItemBase>& _ItemBase)
	{
		TArray<DataTableRowType*> Card_Rows;
		UPlayerStructManager::GetTableRow<DataTableRowType>(_DataTablePath, Card_Rows);
		for (DataTableRowType* Item : Card_Rows)
		{
			_ItemBase.Emplace(((FItemBase)(Item->M_FItemCard)));
		}
	}

	//返回卡片数据数组
	template <class DataTableRowType, class CardType = FItemCard>
	static void GetCardRowDatas(const FString& _DataTablePath, TArray<CardType>& _Cards, int32 _UniformGrade = 0)
	{
		TArray<DataTableRowType*> Card_Rows;
		UPlayerStructManager::GetTableRow<DataTableRowType>(_DataTablePath, Card_Rows);
		for (DataTableRowType* Item : Card_Rows)
		{
			Item->M_FItemCard.M_CardGrade = _UniformGrade;
			_Cards.Emplace(Item->M_FItemCard);
		}
	}

	//创建所有的卡片到背包
	UFUNCTION(BlueprintCallable)
		static void CreateAllCardToBag(UPlayerStructManager* _Player, int32 _UniformCardGrade)
	{
		if (_Player)
			UPlayerStructManager::GetAllCardsData(_Player->M_PlayerItems_Card, _UniformCardGrade);
	}

	//返回一个指定类型的卡片数据
	template<class CardType = FItemCard, class CardStruct = FTableRowBase>
	static void GetCardData(CardType& _CardData, const FString& _Name, const ECardType& _ECardType, const int32& _CardGrade)
	{
		//获取具体卡片数据
		TArray<CardType> Cards;
		UPlayerStructManager::GetCardRowDatas<CardStruct, CardType>(UPlayerStructManager::GetCardDataTablePath(_ECardType), Cards, _CardGrade);

		bool _Result;
		UPlayerStructManager::GetCardArraysData<CardType>(_Name, Cards, _CardData, _Result);
	}

	//返回所有卡片的名称
	static void GetAllCardName(TArray<FItemBase>& _CardName)
	{
		UPlayerStructManager::GetCardName<FCard_ATK_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_ATK), _CardName);
		UPlayerStructManager::GetCardName<FCard_Spawn_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_SPAWN), _CardName);
		UPlayerStructManager::GetCardName<FCard_Defence_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_DEFENCE), _CardName);
		UPlayerStructManager::GetCardName<FCard_Function_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_Function), _CardName);
	}

	//获取卡片数据表所有值
	template <class CardType = FItemCard>
	static void GetCardsDataTable(ECardType _CardType, TArray<CardType>& _Cards, int32 _UniformGrade = 0) {

		if (_CardType == ECardType::E_ATK)
			UPlayerStructManager::GetCardRowDatas<FCard_ATK_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_ATK), _Cards, _UniformGrade);

		if (_CardType == ECardType::E_SPAWN)
			UPlayerStructManager::GetCardRowDatas<FCard_Spawn_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_SPAWN), _Cards, _UniformGrade);

		if (_CardType == ECardType::E_DEFENCE)
			UPlayerStructManager::GetCardRowDatas<FCard_Defence_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_DEFENCE), _Cards, _UniformGrade);

		if (_CardType == ECardType::E_Function)
			UPlayerStructManager::GetCardRowDatas<FCard_Function_Data>(UPlayerStructManager::GetCardDataTablePath(ECardType::E_Function), _Cards, _UniformGrade);
	}

	//搜索卡片查询所有的卡片数据表
	UFUNCTION(BlueprintCallable)
		static bool SearchCardFromDataTable(const FString& _CardName, FItemCard& OutputData, bool _SelectCardType = false, ECardType _CardType = ECardType::E_ATK, int32 _UniformGrade = 0)
	{
		bool _Result = false;

		TArray<FItemCard> Data;

		//攻击
		if (!_SelectCardType || _SelectCardType && _CardType == ECardType::E_ATK)
			UPlayerStructManager::GetCardsDataTable(ECardType::E_ATK, Data, _UniformGrade);
		//生产
		if (!_SelectCardType || _SelectCardType && _CardType == ECardType::E_SPAWN)
			UPlayerStructManager::GetCardsDataTable(ECardType::E_SPAWN, Data, _UniformGrade);
		//防御
		if (!_SelectCardType || _SelectCardType && _CardType == ECardType::E_DEFENCE)
			UPlayerStructManager::GetCardsDataTable(ECardType::E_DEFENCE, Data, _UniformGrade);
		//功能
		if (!_SelectCardType || _SelectCardType && _CardType == ECardType::E_Function)
			UPlayerStructManager::GetCardsDataTable(ECardType::E_Function, Data, _UniformGrade);

		UPlayerStructManager::GetCardArraysData<FItemCard>(_CardName, Data, OutputData, _Result);

		return _Result;
	}

	//返回所有卡片的数据
	UFUNCTION(BlueprintCallable)
		static void GetAllCardsData(TArray<FItemCard>& _CardDatas, int32 _UniformGrade = 0)
	{
		UPlayerStructManager::GetCardsDataTable(ECardType::E_ATK, _CardDatas, _UniformGrade);
		UPlayerStructManager::GetCardsDataTable(ECardType::E_SPAWN, _CardDatas, _UniformGrade);
		UPlayerStructManager::GetCardsDataTable(ECardType::E_DEFENCE, _CardDatas, _UniformGrade);
		UPlayerStructManager::GetCardsDataTable(ECardType::E_Function, _CardDatas, _UniformGrade);
	}
	/*-----------------------------------------------卡片-----------------------------------------------*/
	/*-----------------------------------------------卡片-----------------------------------------------*/
};

//货币计算
USTRUCT(BlueprintType)
struct FPlayerCoinAdd
{
	GENERATED_USTRUCT_BODY()
public:
	FPlayerCoinAdd() {}

	static bool Buy(UPlayerStructManager* _Player, const int32& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		//添加货币
		switch (_CoinGrade)
		{

		case 0:
		{
			if (_Player->M_FPlayerCoin.M_Coin_0 < _Num)
				return false;

			_Player->M_FPlayerCoin.M_Coin_0 -= _Num;
			return true;
		}
		case 1:
		{
			if (_Player->M_FPlayerCoin.M_Coin_1 < _Num)
				return false;

			_Player->M_FPlayerCoin.M_Coin_1 -= _Num;
			return true;
		}
		case 2:
		{
			if (_Player->M_FPlayerCoin.M_Coin_2 < _Num)
				return false;

			_Player->M_FPlayerCoin.M_Coin_2 -= _Num;
			return true;
		}
		}

		return false;
	}

	static bool Sell(UPlayerStructManager* _Player, const int32& _Num, const uint8& _CoinGrade) {
		if (!_Player)
			return false;

		//添加货币
		switch (_CoinGrade)
		{
		case 0:
			_Player->M_FPlayerCoin.M_Coin_0 += _Num;
			return true;
		case 1:
			_Player->M_FPlayerCoin.M_Coin_1 += _Num;
			return true;
		case 2:
			_Player->M_FPlayerCoin.M_Coin_2 += _Num;
			return true;
		}

		return false;
	}
};
