// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include <Components/Button.h>
#include "UI_SelectTip.generated.h"

/**
 *
 */

class UButton;

UCLASS()
class FVM_API UUI_SelectTip : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UButton* M_UButton = nullptr;
public:
	//标题
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_Title = FText();
	//内容文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_Content = FText();
	//确认按钮的文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_Button_Ok_Text = FText(FText::FromString(TEXT("确认")));
	//取消按钮的文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_Button_Cancel_Text = FText(FText::FromString(TEXT("关闭")));
public:
	virtual bool Initialize() override;
public:
	FOnButtonClickedEvent& GetButtonClickBind();
public:

	UFUNCTION(BlueprintCallable)
		UButton* GetOkButton();

	UFUNCTION(BlueprintCallable)
		static UUI_SelectTip* CreateSelectTip(UWidget* _Owner = nullptr);
};
