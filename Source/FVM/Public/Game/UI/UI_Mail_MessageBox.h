// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Mail_MessageBox.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UUI_Mail;

UCLASS()
class FVM_API UUI_Mail_MessageBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	//按钮状态
	int32 M_MailStateValue = 0;
private:
	UPROPERTY()
		UButton* M_MailState = nullptr;
	UPROPERTY()
		UImage* M_MailHead = nullptr;
	UPROPERTY()
		UImage* M_MailLog = nullptr;
public:
	UUI_Mail* M_Mail = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MailTitle;
public:
	//打开邮件
	UFUNCTION()
		void OpenMail();
public:
	virtual bool Initialize() override;
	//设置Mail
	void SetMail(UUI_Mail* _Mail);
	//获取Mail
	UUI_Mail* const GetMail();
	//设置按钮状态
	void SetMailStateTexture(int32 _MailState);
	//设置按钮状态
	void SetMailState(int32 _MailStateValue);
	//获取按钮状态
	int32 GetMailState();
};
