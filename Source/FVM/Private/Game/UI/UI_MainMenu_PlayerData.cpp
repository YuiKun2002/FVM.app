// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainMenu_PlayerData.h"
#include "Game/UI/UI_MainMenu_InGame.h"
#include "GameSystem/FVMGameInstance.h"

#include <Components/TextBlock.h>

#include "GameSystem/GameConfigManager.h"
#include "GameSystem/PlayerStructManager.h"

#include "Game/UI/Tips/UI_SelectTip.h"

void UUI_MainMenu_PlayerData::UpdateData()
{
	if (this->M_UI_MainMenu_InGame)
		this->M_UI_MainMenu_InGame->UpdatePlayerTextShow();

	//保存游戏存档
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UFVMGameInstance::GetFVMGameInstance()->SaveDataToGameConfigManager();
	}
}

bool UUI_MainMenu_PlayerData::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_TextName = this->GetWidgetComponent<UTextBlock>(this, "TextBlock_47");

	return true;
}

bool UUI_MainMenu_PlayerData::SelectPlayer()
{
	FString& CurrentName = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName;

	//名称相同(当前选择选项卡名称和存档当前选择的名称)
	if (CurrentName.Equals(this->M_PlayerName.ToString()))
		return true;

	//从本地寻找(当前选项卡选择的名称)如果没有则加载失败
	if (!UFVMGameInstance::GetFVMGameInstance()->IsFileExist(this->M_PlayerName.ToString() + FString("P")))
		return false;

	//加载当前的存档
	if (!UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(this->M_PlayerName.ToString()))
	{
		//如果加载失败了，则加载默认存档
		CurrentName = FString("");

		//加载默认角色存档
		UFVMGameInstance::GetFVMGameInstance()->InitPlayerStructManager();

		this->UpdateData();

		return false;
	}

	//将当前选择的存档名称更新
	CurrentName = M_PlayerName.ToString();

	//更新数据
	this->UpdateData();

	return true;
}

void UUI_MainMenu_PlayerData::PlayerClickDelet()
{
	//创建提示
	UUI_SelectTip* Tip = UUI_SelectTip::CreateSelectTip(this);
	Tip->M_Title = FText::FromString(TEXT("角色存档操作"));
	Tip->M_Content = FText::FromString(TEXT("是否要删除这个存档?此操作不可逆!"));
	Tip->M_Button_Ok_Text = FText::FromString(TEXT("删除"));
	Tip->M_Button_Cancel_Text = FText::FromString(TEXT("取消"));
	FScriptDelegate Click;
	Click.BindUFunction(this, "DeleteAll");
	Tip->GetButtonClickBind().Add(Click);
	//AddDynamic(this, &UUI_MainMenu_PlayerData::DeletePlayerData);
}

bool UUI_MainMenu_PlayerData::DeletePlayerData()
{
	//获取当前从列表中选择的存档名称(当前正在使用的存档名称)
	FString& CurrentName = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName;

	//从列表中查询(查询当前选择删除的选项卡名称)
	const int32& FileIndex = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerStructManagerName.Find(this->M_PlayerName.ToString());

	//如果从列表中查询到匹配名称
	if (FileIndex != -1)
	{
		//如果和选择的存档名称相同则加载默认存档
		if (this->M_PlayerName.ToString().Equals(CurrentName))
		{
			//将当前选择的选项卡删除
			CurrentName = FString("");
			//加载默认角色存档
			UFVMGameInstance::GetFVMGameInstance()->InitPlayerStructManager();

		}

		//删除存档文件
		UFVMGameInstance::GetFVMGameInstance()->DeletePlayerStructConfig(this->M_PlayerName.ToString());

		UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerStructManagerName.RemoveAt(FileIndex);
		UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerStructManagerSex.RemoveAt(FileIndex);

		this->UpdateData();
		return true;
	}

	return false;
}

void UUI_MainMenu_PlayerData::DeleteAll()
{
	this->DeletePlayerData();
	this->RemoveFromParent();
}

void UUI_MainMenu_PlayerData::SetTextPink()
{
	//更新字体颜色：女:粉
	//if (UFVMGameInstance::GetFVMGameInstance() && UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex == 0)
	this->M_TextName->SetColorAndOpacity(FSlateColor(FLinearColor(1.6f, 0.2f, 0.4f, 1.f)));
}
