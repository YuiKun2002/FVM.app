// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_BagRemove_Equipment.h"
#include  <Components/HorizontalBox.h>
bool UUI_BagRemove_Equipment::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_EquipmentHead = this->GetWidgetComponent<UImage>(this, "Head");
	this->M_EquipmentCountPanel = this->GetWidgetComponent<UHorizontalBox>(this, "HorizontalBox_114");

	return true;
}

void UUI_BagRemove_Equipment::SetEquipment(const FString& _HeadPath, bool _IsOverlap, int32 _ItemCount)
{
	UWidgetBase::SetImageBrush(this->M_EquipmentHead, _HeadPath);

	if (_IsOverlap)
	{
		M_EquipmentNum = FString("x") + FString::FormatAsNumber(_ItemCount);
	}
	else
	{
		M_EquipmentNum = FString();
		this->M_EquipmentCountPanel->SetVisibility(ESlateVisibility::Hidden);
	}
}
