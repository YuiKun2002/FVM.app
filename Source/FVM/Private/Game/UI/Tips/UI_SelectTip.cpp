// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/Tips/UI_SelectTip.h"

bool UUI_SelectTip::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UButton = this->GetWidgetComponent<UButton>(this, "Ok");

	return true;
}

FOnButtonClickedEvent& UUI_SelectTip::GetButtonClickBind()
{
	return M_UButton->OnClicked;
}

UButton* UUI_SelectTip::GetOkButton()
{
	return this->M_UButton;
}

UUI_SelectTip* UUI_SelectTip::CreateSelectTip(UWidget* _Owner)
{
	UUI_SelectTip* Widget = CreateWidget<UUI_SelectTip>(_Owner, LoadClass<UUI_SelectTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_SelectTip.UI_SelectTip_C'")));
	Widget->AddToViewport();
	return Widget;
}
