// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ShopItemPriceStruct.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Item/ItemStruct.h"
#include "Data/MaterialDataStruct.h"


void UShopItemPriceStruct::AddToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count)
{
	if (_Player)
	{
		switch (_ItemData.M_ItemType)
		{
		case EItemType::E_Card:UShopItemPriceStruct::AddCardToPlayerBag(_Player, _ItemData, _Count); break;
		case EItemType::E_Equipment:UShopItemPriceStruct::AddEquipmentToPlayerBag(_Player, _ItemData, _Count); break;
		case EItemType::E_MaterialItem:UShopItemPriceStruct::AddMaterialToPlayerBag(_Player, _ItemData, _Count); break;
		}
	}
}

void UShopItemPriceStruct::AddPlayerBagGrid(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Panel_Index)
{
	if (_Player && _ItemData.M_ItemType == EItemType::E_BagGrid)
	{
		switch (_Panel_Index)
		{
		case 0:_Player->M_PlayerBagGirdGroup.M_EquipmentGrid.Emplace(FPlayerBagGird()); break;
		case 1:_Player->M_PlayerBagGirdGroup.M_CardGrid.Emplace(FPlayerBagGird()); break;
		case 2:_Player->M_PlayerBagGirdGroup.M_MaterialGrid.Emplace(FPlayerBagGird()); break;
		}
	}
}

void UShopItemPriceStruct::AddCardToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count)
{
	FItemCard _CardData;

	if (UPlayerStructManager::SearchCardFromDataTable(_ItemData.M_ItemName, _CardData))
	{
		for (int32 i = 0; i < _Count; i++)
		{
			_Player->M_PlayerItems_Card.Emplace(_CardData);
		}
		//UE_LOG(LogTemp, Error, TEXT("%s"), *_ItemData.M_ItemName);
	}
}

void UShopItemPriceStruct::AddEquipmentToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count)
{
	FEquipmentBase  _FEquipment;

	if (UPlayerStructManager::SearchEquipmentFromDataTable(_ItemData.M_ItemName, _FEquipment))
	{
		//��ǰ���߿��ص�
		if (_FEquipment.M_IsOverlap)
		{
			//�ȴӱ�����ѯ->�����ѯ�ɹ���ֱ����Ӹ���
			for (FEquipmentBase& Items : _Player->M_PlayerItems_Equipment)
			{
				if (Items.ItemName.Equals(_FEquipment.ItemName))
				{
					Items.M_Count += _Count;
					return;
				}
			}

			//�����ѯʧ��->����ӵ�����Ȼ��ֱ���޸ĸ���
			_FEquipment.M_Count = _Count;
			_Player->M_PlayerItems_Equipment.Emplace(_FEquipment);

			return;
		}

		for (int32 i = 0; i < _Count; i++)
		{
			//�жϵ�ǰ���߲����ص�
			_Player->M_PlayerItems_Equipment.Emplace(_FEquipment);
		}

	}

}

void UShopItemPriceStruct::AddMaterialToPlayerBag(UPlayerStructManager* _Player, const FItemPrice& _ItemData, int32 _Count)
{
	FMaterialBase _FMaterialBase;

	if (UPlayerStructManager::SearchMaterailFromDataTable(_ItemData.M_ItemName, _FMaterialBase))
	{
		//�ȴӱ�����ѯ->�����ѯ�ɹ���ֱ����Ӹ���
		for (FMaterialBase& Items : _Player->M_PlayerItems_Material)
		{
			if (Items.ItemName.Equals(_FMaterialBase.ItemName))
			{
				Items.M_Count += _Count;
				return;
			}
		}

		//�����ѯʧ��->����ӵ�����Ȼ��ֱ���޸ĸ���
		_FMaterialBase.M_Count = _Count;
		_Player->M_PlayerItems_Material.Emplace(_FMaterialBase);
	}
}
