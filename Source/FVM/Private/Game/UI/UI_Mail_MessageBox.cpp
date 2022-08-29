// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Mail_MessageBox.h"
#include "Game/UI/UI_Mail_GiftBox.h"
#include "Game/UI/UI_Mail.h"
#include <Components/Image.h>
#include <Components/Button.h>

void UUI_Mail_MessageBox::OpenMail()
{
	this->SetMailStateTexture(this->GetMailState());
	UUI_Mail_GiftBox* Gift = CreateWidget<UUI_Mail_GiftBox>(this, LoadClass<UUI_Mail_GiftBox>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_Mail_GiftBox.BPUI_Mail_GiftBox_C'")));
	Gift->SetMailBox(this);
	this->GetMail()->AddGiftToPanel(Gift);
	Gift->Show();
}

bool UUI_Mail_MessageBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_MailState = this->GetWidgetComponent<UButton>(this, "Mail_Look_Butt");
	this->M_MailHead = this->GetWidgetComponent<UImage>(this, "Mail_Head");
	this->M_MailLog = this->GetWidgetComponent<UImage>(this, "Mail_Log");

	//绑定按钮
	this->M_MailState->OnClicked.AddDynamic(this, &UUI_Mail_MessageBox::OpenMail);

	return true;
}

void UUI_Mail_MessageBox::SetMail(UUI_Mail* _Mail)
{
	this->M_Mail = _Mail;
}

UUI_Mail* const UUI_Mail_MessageBox::GetMail()
{
	return this->M_Mail;
}

void UUI_Mail_MessageBox::SetMailStateTexture(int32 _MailState)
{
	if (_MailState == 1)
	{
		UWidgetBase::SetButtonStyle(this->M_MailState, "Texture2D'/Game/Resource/Texture/UI/Game/Mail/T_EM_6.T_EM_6'");
	}
	else if (_MailState == 2)
	{
		UWidgetBase::SetButtonStyle(this->M_MailState, "Texture2D'/Game/Resource/Texture/UI/Game/Mail/T_EM_7.T_EM_7'");
	}
	else {
		UWidgetBase::SetButtonStyle(this->M_MailState, "Texture2D'/Game/Resource/Texture/UI/Game/Mail/T_EM_5.T_EM_5'");
	}
}

void UUI_Mail_MessageBox::SetMailState(int32 _MailStateValue)
{
	this->M_MailStateValue = _MailStateValue;
}

int32 UUI_Mail_MessageBox::GetMailState()
{
	return this->M_MailStateValue;
}
