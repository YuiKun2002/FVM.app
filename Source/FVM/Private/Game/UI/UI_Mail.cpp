// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Mail.h"
#include "Game/UI/UI_Mail_GiftBox.h"
#include "Game/UI/UI_Mail_MessageBox.h"

#include <Components/Image.h>
#include <Components/Button.h>
#include <Components/MultiLineEditableText.h>
#include <Components/HorizontalBox.h>
#include <Components/VerticalBox.h>

void UUI_Mail::SendMsgToSystem()
{
	this->AnalysisMsg(this->M_PlayerInputContent->GetText().ToString());
	this->M_PlayerInputContent->SetText(FText());
}

void UUI_Mail::AnalysisMsg(FString _Value)
{
	//临时占用
	this->CreateNewMailToList();
}

bool UUI_Mail::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_MailListBox = this->GetWidgetComponent<UVerticalBox>(this, "VerticalBox_117");
	this->M_MailItemBox = this->GetWidgetComponent<UHorizontalBox>(this, "Maill_Gift_Panel");
	this->M_PlayerInputContent = this->GetWidgetComponent<UMultiLineEditableText>(this, "Input_Content_TextLine");
	this->M_PlayerSendMsgButt = this->GetWidgetComponent<UButton>(this, "SendMsg");


	//绑定按钮事件
	this->M_PlayerSendMsgButt->OnClicked.AddDynamic(this, &UUI_Mail::SendMsgToSystem);

	//初始化信息
	this->InitializeMsgText();

	return true;
}

void UUI_Mail::InitializeMsgText()
{
	this->M_MailReceivePlayerName = TEXT("亲爱的你们");
	this->M_MailThemeTitle = TEXT("来自开发者的信");
	this->M_MailContent = TEXT("1.这个游戏是哔哩哔哩赖小宇ly一人制作的单机版游戏-美食大战老鼠。\n 2.这个游戏是免费游戏, 不收费并且开源, 作为Up的一个童年游戏, 还是花了一定时间来制作的, 因为是一个人做没有更多时间打磨细节希望理解");
}

void UUI_Mail::CreateNewMailToList()
{
	UUI_Mail_MessageBox* MailMsg = CreateWidget<UUI_Mail_MessageBox>(this, LoadClass<UUI_Mail_MessageBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Mail_MessageBox.BPUI_Mail_MessageBox_C'")));
	MailMsg->SetMail(this);
	this->M_MailListBox->AddChildToVerticalBox(MailMsg);
}

void UUI_Mail::SetMailContent(const FString& _RecvName, const FString& _ThemeTile, const FString& _Content)
{
	this->M_MailReceivePlayerName = _RecvName;
	this->M_MailThemeTitle = _ThemeTile;
	this->M_MailContent = _Content;
}

void UUI_Mail::AddGiftToPanel(UWidget* _Widget)
{
	this->M_MailItemBox->ClearChildren();
	this->M_MailItemBox->AddChildToHorizontalBox(_Widget);
}
