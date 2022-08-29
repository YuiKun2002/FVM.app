// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MouseRound.generated.h"

class USlider;
class UTextBlock;
class UImage;
class UCanvasPanel;
class UVerticalBox;

UCLASS()
class FVM_API UUI_MouseRound : public UWidgetBase
{
	GENERATED_BODY()
public:
	//老鼠进度条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USlider* M_USlider = nullptr;
	//老鼠进攻回合文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextBlock* M_MouseRoundText = nullptr;
	//boss鼠列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UVerticalBox* M_Boss_List_VerticalBox = nullptr;

public:
	virtual bool Initialize() override;

	//更新老鼠进度条
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundSlider(const float& _Curr, const float& _End);

	//更新老鼠回合文字
	UFUNCTION(BlueprintCallable)
		void UpdateMouseRoundText(const int32& _Round);

	//将boss的血量界面添加到列表中
	UFUNCTION(BlueprintCallable)
		void AddMouseBossSlider(UWidget* _Widget_);
};
