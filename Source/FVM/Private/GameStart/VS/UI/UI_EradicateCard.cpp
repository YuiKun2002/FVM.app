// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_EradicateCard.h"
#include <Components/Button.h>

bool UUI_EradicateCard::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_Eradicate = this->GetWidgetComponent<UButton>(this, "Eradicate");

	return true;
}

UButton* UUI_EradicateCard::GetButton()
{
	return this->M_Eradicate;
}
