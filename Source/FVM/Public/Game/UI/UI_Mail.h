// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Mail.generated.h"

/**
 *
 */

class UImage;
class UButton;
class UMultiLineEditableText;
class UHorizontalBox;
class UVerticalBox;

UCLASS()
class FVM_API UUI_Mail : public UWidgetBase
{
	GENERATED_BODY()
private:
	//邮件列表
	UPROPERTY()
		UVerticalBox* M_MailListBox = nullptr;
	//邮件发送的物品区域
	UPROPERTY()
		UHorizontalBox* M_MailItemBox = nullptr;
	//玩家输入信息的多行输入框
	UPROPERTY()
		UMultiLineEditableText* M_PlayerInputContent = nullptr;
	//发送信息的按钮
	UPROPERTY()
		UButton* M_PlayerSendMsgButt = nullptr;
public:
	//收件人名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MailReceivePlayerName;
	//主题标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MailThemeTitle;
	//邮件内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MailContent;
public:
	//发送信息给系统
	UFUNCTION()
		void SendMsgToSystem();
	//解析信息
	UFUNCTION()
		void AnalysisMsg(FString _Value);
public:
	virtual bool Initialize() override;
	//初始化信息显示列表
	void InitializeMsgText();
	//创建新的邮件到列表
	void CreateNewMailToList();
	//设置邮件内容
	void SetMailContent(const FString& _RecvName, const FString& _ThemeTile, const FString& _Content);
	//将礼物添加到显示界面
	void AddGiftToPanel(UWidget* _Widget);
};
