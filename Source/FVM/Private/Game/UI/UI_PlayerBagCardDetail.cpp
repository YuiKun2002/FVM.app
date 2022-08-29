// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagCardDetail.h"
#include "Game/UI/Tips/UI_SelectTip.h"
#include <Components/Button.h>

#include "Game/UI/UI_PlayerBag.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

bool UUI_PlayerBagCardDetail::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_RemoveCard = this->GetWidgetComponent<UButton>(this, "RemoveCard");


	this->M_RemoveCard->OnClicked.AddDynamic(this, &UUI_PlayerBagCardDetail::CreateTip);

	return true;
}

void UUI_PlayerBagCardDetail::CreateTip()
{
	UUI_SelectTip* Tip = CreateWidget<UUI_SelectTip>(this, LoadClass<UUI_SelectTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_SelectTip.UI_SelectTip_C'")));
	Tip->M_Button_Ok_Text = FText(FText::FromString(TEXT("删除")));
	Tip->M_Button_Cancel_Text = FText(FText::FromString(TEXT("手滑了QAQ")));
	Tip->M_Title = FText(FText::FromString(TEXT("删除卡片") + this->ItemName));
	Tip->M_Content = FText(FText::FromString(TEXT("当前卡片会从你的背包中被删除，真的想好了吗？")));
	FScriptDelegate CallFunc;
	CallFunc.BindUFunction(this, "RemoveCurrent");
	Tip->GetButtonClickBind().Add(CallFunc);
	Tip->AddToViewport();
}

void UUI_PlayerBagCardDetail::RemoveCurrent()
{
	for (auto PP = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.CreateIterator(); PP; PP++)
	{
		if (&(*PP) == M_FItemCard)
		{
			PP.RemoveCurrent();
			break;
		}
	}

	//保存数据
	UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
	
	this->RemoveFromParent();

	M_PlayerUIBag->LoadCardData();
}
