// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_MainMenu_InGame.generated.h"

/**
 *  首次进入游戏出现的界面  主菜单
 */

class UScrollBox;
class UEditableTextBox;

UCLASS()
class FVM_API UUI_MainMenu_InGame : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UScrollBox* M_UScrollBox_PlayerDataList = nullptr;
	UPROPERTY()
		UEditableTextBox* M_UEditableTextBox_PlayerInput = nullptr;
public:
	//用于显示当前选择的存档名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText M_CurrentPlayerDataName;
	//选择的性别
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 M_PlayerSex = 0;
public:
	virtual bool Initialize() override;
	//添加列表
	void AddList(UClass* _Obj,const FString& __PlayerName,uint8 _Sex);
public:
	UFUNCTION(BlueprintCallable)
		void InitPlayerData();
	//创建新的角色
	UFUNCTION(BlueprintCallable)
		bool CreateNewPlayerData();
	//更新玩家选择的存档文字显示
	UFUNCTION(BlueprintCallable)
		void UpdatePlayerTextShow();
	//检查角色存档是否有效
	UFUNCTION(BlueprintCallable)
		bool InspectPlayerData();
};
