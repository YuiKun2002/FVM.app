// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_Mail_GiftBox.generated.h"

/**
 *
 */

class UTextBlock;
class UImage;
class UButton;
class UUI_Mail;
class UUI_Mail_MessageBox;

UCLASS()
class FVM_API UUI_Mail_GiftBox : public UWidgetBase
{
	GENERATED_BODY()
private:
	//物品的图形
	UPROPERTY()
		UImage* M_ItemHead = nullptr;
	//领取按钮
	UPROPERTY()
		UButton* M_Receive_Butt = nullptr;
	//删除按钮
	UPROPERTY()
		UButton* M_Remove_Butt = nullptr;
private:
	UUI_Mail_MessageBox* M_MailBox = nullptr;
public:
	//物品的名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName;
public:
	virtual bool Initialize() override;
public:
	//设置MailBox
	void SetMailBox(UUI_Mail_MessageBox* _MailBox);
	//获取MailBox
	UUI_Mail_MessageBox* const GetMailBox();
public:
	//领取奖励按钮功能
	UFUNCTION()
		void Receive();
	//删除邮件按钮功能
	UFUNCTION()
		void Remove();
	//销毁邮件
	UFUNCTION()
		void DestroyMail();
	//获取领取按钮
	UFUNCTION()
		UButton* GetReceiveButton();
	//获取删除按钮
	UFUNCTION()
		UButton* GetRemoveButton();
	//获取图片
	UFUNCTION()
		UImage* GetItemHead();
	//显示
	UFUNCTION()
		void Show();
};
