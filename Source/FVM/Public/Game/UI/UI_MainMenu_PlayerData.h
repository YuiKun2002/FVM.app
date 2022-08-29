// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MainMenu_PlayerData.generated.h"

/**
 * 角色信息显示
 */

class UUI_MainMenu_InGame;
class UTextBlock;

UCLASS()
class FVM_API UUI_MainMenu_PlayerData : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UTextBlock* M_TextName = nullptr;
public:
	//存档名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_PlayerName;
	//主菜单UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_MainMenu_InGame* M_UI_MainMenu_InGame = nullptr;
private:
	void UpdateData();
public:
	virtual bool Initialize() override;
public:
	//选择角色
	UFUNCTION(BlueprintCallable)
		bool SelectPlayer();

	//玩家准备删除存档的时候
	UFUNCTION(BlueprintCallable)
		void PlayerClickDelet();

	//删除角色数据(存档)
	UFUNCTION(BlueprintCallable)
		bool DeletePlayerData();

	//删除数据包括当前UI界面
	UFUNCTION(BlueprintCallable)
		void DeleteAll();

	//设置粉色字体
	UFUNCTION(BlueprintCallable)
		void SetTextPink();
};
