// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include <Components/Button.h>
#include "GameSystem/Item/ItemStruct.h"
#include "UI_PlayerBagCardGrid.generated.h"

class UUI_PlayerSynthesis;
class UUI_PlayerBag;
class UButton;
class UImage;

UCLASS()
class FVM_API UUI_PlayerBagCardGrid : public UWidgetBase
{
	GENERATED_BODY()
private:
	//卡片数据（不允许地址被修改，使用前必须判断地址有效性）
	FItemCard* M_FItemCard = nullptr;
	//合成屋的UI
	UUI_PlayerSynthesis* M_UI_PlayerSynthesis = nullptr;
public:
	UUI_PlayerBag* M_PlayerUIBag = nullptr;
public:
	//卡片价格文字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_CardPriceText;
	//卡片图片路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardTexturePath;
private:
	//卡片按钮
	UPROPERTY()
		UButton* M_CardButton = nullptr;
	//卡片等级图片
	UPROPERTY()
		UImage* M_CardGradeImage = nullptr;
public:
	virtual bool Initialize() override;

	//获取按钮点击事件
	FOnButtonClickedEvent& GetButtonClickEvent();
	//设置卡片数据
	void SetFItemCardData(FItemCard* const _FItemCardData);
	//设置合成屋的UI
	void SetUI_PlayerSynthesis(UUI_PlayerSynthesis* const _PlayerSynthesis);
	//获取卡片数据
	FItemCard* const GetFItemCardData();
public:
	//显示卡片的细节
	UFUNCTION()
		void ShowCardDetails();

	//选择当前卡片(游戏准备-选卡)
	UFUNCTION()
		void SelectCurrentCard();
	//移除当前选择的卡片
	UFUNCTION()
		void RemoveCurrentSelectCard();

	/*----------------------------------------卡片强化----------------------------------------*/
	//添加合成屋的强化用卡卡槽
	UFUNCTION()
		void AddSynthesisUpGrade();
	/*----------------------------------------卡片强化----------------------------------------*/
	/*----------------------------------------卡片转职----------------------------------------*/
	UFUNCTION()
		void AddSynthesisChangeJobs();
	/*----------------------------------------卡片转职----------------------------------------*/


public:
	//获取卡片
	UFUNCTION(BlueprintCallable)
		UButton* GetButton();

	//更新按钮的纹理
	UFUNCTION(BlueprintCallable)
		void UpdateButtonTexture(const FString& _Price);

	//获取卡片纹理
	UFUNCTION(BlueprintCallable)
		UTexture2D* GetTexture();

	//播放动画
	UFUNCTION(BlueprintCallable)
		void PlayBeginAnim();
};
