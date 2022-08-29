// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_CardBar.h"
#include <Components/HorizontalBox.h>

UHorizontalBox* UUI_CardBar::GetCardList()
{
	return this->M_UHorizontalBox;
}

bool UUI_CardBar::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UHorizontalBox = this->GetWidgetComponent<UHorizontalBox>(this,"CardList",F);

	return true;
}
