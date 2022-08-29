// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_TableTextBlock.h"

bool UUI_TableTextBlock::Initialize()
{
	if (!Super::Initialize())
		return false;



	return true;
}

void UUI_TableTextBlock::SetText(const FString& _Content)
{
	this->M_Title = _Content;
}
