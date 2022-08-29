// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_Mail_GiftBox.h"
#include "Game/UI/UI_Mail.h"
#include "Game/UI/UI_Mail_MessageBox.h"
#include "Game/UI/Tips/UI_SelectTip.h"
#include <Components/Button.h>
#include <Components/Image.h>

bool UUI_Mail_GiftBox::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化组件
	this->M_ItemHead = this->GetWidgetComponent<UImage>(this, "Mail_Log");
	this->M_Receive_Butt = this->GetWidgetComponent<UButton>(this, "Receive_Butt");
	this->M_Remove_Butt = this->GetWidgetComponent<UButton>(this, "RemoveMail_Butt");

	this->M_Receive_Butt->OnClicked.AddDynamic(this, &UUI_Mail_GiftBox::Receive);
	this->M_Remove_Butt->OnClicked.AddDynamic(this, &UUI_Mail_GiftBox::Remove);

	return true;
}

void UUI_Mail_GiftBox::SetMailBox(UUI_Mail_MessageBox* _MailBox)
{
	this->M_MailBox = _MailBox;
}

UUI_Mail_MessageBox* const UUI_Mail_GiftBox::GetMailBox()
{
	return this->M_MailBox;
}

void UUI_Mail_GiftBox::Receive()
{
	this->M_Receive_Butt->SetIsEnabled(false);

	this->GetMailBox()->SetMailState(2);
	this->GetMailBox()->SetMailStateTexture(2);

	FString Tip = TEXT("领取成功");
	UWidgetBase::CreateTipWidget(Tip);
}

void UUI_Mail_GiftBox::Remove()
{
	UUI_SelectTip* Tip = CreateWidget<UUI_SelectTip>(this, LoadClass<UUI_SelectTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_SelectTip.UI_SelectTip_C'")));
	Tip->M_Title = FText::FromString(TEXT("邮件删除"));
	Tip->M_Content = FText::FromString(TEXT("确定要把这封信件删除吗？"));
	Tip->M_Button_Ok_Text = FText::FromString(TEXT("删除"));
	Tip->M_Button_Cancel_Text = FText::FromString(TEXT("先等等"));
	FScriptDelegate Func;
	Func.BindUFunction(this, "DestroyMail");
	Tip->GetOkButton()->OnClicked.Add(Func);
	Tip->AddToViewport();
}

void UUI_Mail_GiftBox::DestroyMail()
{
	this->GetMailBox()->GetMail()->InitializeMsgText();
	this->GetMailBox()->RemoveFromParent();
	this->RemoveFromParent();
}

UButton* UUI_Mail_GiftBox::GetReceiveButton()
{
	return this->M_Receive_Butt;
}

UButton* UUI_Mail_GiftBox::GetRemoveButton()
{
	return this->M_Remove_Butt;
}

UImage* UUI_Mail_GiftBox::GetItemHead()
{
	return this->M_ItemHead;
}

void UUI_Mail_GiftBox::Show()
{
	this->GetMailBox()->GetMail()->SetMailContent(TEXT("小苏苏"), TEXT("临时的书信"), TEXT("嗯？"));

	if (this->GetMailBox()->GetMailState() == 2)
		this->M_Receive_Butt->SetIsEnabled(false);

	if (this->GetMailBox()->GetMailState() != 1)
	{
		if (this->GetMailBox()->GetMailState() == 2)
			return;
		this->GetMailBox()->SetMailState(1);
		this->GetMailBox()->SetMailStateTexture(1);
	}
}
