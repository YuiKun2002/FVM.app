// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_FlameManager.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"
#include <Components/TextBlock.h>

bool UUI_FlameManager::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_FlameNumText = this->GetWidgetComponent<UTextBlock>(this, "FlameNum");

	return true;
}

void UUI_FlameManager::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (this->M_FlameNum != this->GetFlameNum())
	{
		this->M_FlameNum = this->GetFlameNum();
		this->M_FlameNumText->SetText(FText::FromString(FString::FormatAsNumber(this->M_FlameNum)));
	}

}

int32 UUI_FlameManager::GetFlameNum()
{
	return AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->M_FlameNum;
}
