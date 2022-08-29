// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_MainMenu_InGame.h"
#include "Game/UI/UI_MainMenu_PlayerData.h"

#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "GameSystem/FVMGameInstance.h"

#include <Components/ScrollBox.h>
#include <Components/EditableTextBox.h>

void UUI_MainMenu_InGame::InitPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	if (!Config)
		return;

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));
	UUI_MainMenu_PlayerData* Data = nullptr;

	//加载数据
	for (int32 Index = 0; Index < Config->M_PlayerStructManagerName.Num(); Index++)
	{
		Data = CreateWidget<UUI_MainMenu_PlayerData>(this, ObjInstance);
		Data->M_PlayerName = FText::FromString(Config->M_PlayerStructManagerName[Index]);
		Data->M_UI_MainMenu_InGame = this;

		if (Config->M_PlayerStructManagerSex[Index] == 0)
			Data->SetTextPink();

		this->M_UScrollBox_PlayerDataList->AddChild(Data);
	}

	//初始化玩家当前选择的存档名称
	this->UpdatePlayerTextShow();
}

bool UUI_MainMenu_InGame::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化
	this->M_UScrollBox_PlayerDataList = this->GetWidgetComponent<UScrollBox>(this, "PlayerDataList_Scroll");
	this->M_UEditableTextBox_PlayerInput = this->GetWidgetComponent<UEditableTextBox>(this, "Input_Player_Name");

	//初始化角色数据
	//this->InitPlayerData(); 

	return true;
}

void UUI_MainMenu_InGame::AddList(UClass* _Obj, const FString& __PlayerName, uint8 _Sex)
{
	UUI_MainMenu_PlayerData* Data = CreateWidget<UUI_MainMenu_PlayerData>(this, _Obj);
	Data->M_PlayerName = FText::FromString(__PlayerName);
	Data->M_UI_MainMenu_InGame = this;
	if (_Sex == 0)
		Data->SetTextPink();
	this->M_UScrollBox_PlayerDataList->AddChild(Data);
}
 
bool UUI_MainMenu_InGame::CreateNewPlayerData()
{
	// UE_LOG(LogTemp, Error, TEXT("A"));

	//输入的文字是空的
	if (this->M_UEditableTextBox_PlayerInput->GetText().ToString().Equals(""))
		return false;

	//获取游戏数据
	if (!UFVMGameInstance::GetFVMGameInstance())
		return false;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();

	const FString& _PlayerName = this->M_UEditableTextBox_PlayerInput->GetText().ToString();

	//从存档列表查询是否存在
	if (Config->M_PlayerStructManagerName.Find(_PlayerName) != -1)
		return false;

	//加载对应的界面
	UClass* ObjInstance = LoadClass<UUI_MainMenu_PlayerData>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/InGame/UI_InGame_PlayerData.UI_InGame_PlayerData_C'"));

	//从本地查询是否存在文件
	if (UFVMGameInstance::GetFVMGameInstance()->IsFileExist(_PlayerName + FString("P")))
	{
		//如果已经存在了，直接加载到列表
		Config->M_PlayerStructManagerName.Emplace(_PlayerName);
		Config->M_CurrentPlayerName = _PlayerName;
		UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(_PlayerName);
		//性别加载
		Config->M_PlayerStructManagerSex.Emplace(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex);

		this->AddList(ObjInstance, _PlayerName, UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex);
	}
	else {
		//添加角色
		Config->M_PlayerStructManagerName.Emplace(_PlayerName);
		Config->M_PlayerStructManagerSex.Emplace(this->M_PlayerSex);
		Config->M_CurrentPlayerName = _PlayerName;

		UFVMGameInstance::GetFVMGameInstance()->CreatePlayerStructConfig(_PlayerName);

		//将当前创建的新的角色存档就行修改和保存
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerName = _PlayerName;
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerSex = this->M_PlayerSex;
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_GameVersion = UFVMGameInstance::GetGameVersion();
		UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(_PlayerName);

		this->AddList(ObjInstance, _PlayerName, this->M_PlayerSex);
	}

	this->UpdatePlayerTextShow();
	return true;
}

void UUI_MainMenu_InGame::UpdatePlayerTextShow()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	//初始化玩家当前选择的存档名称
	this->M_CurrentPlayerDataName = FText::FromString(Config->M_CurrentPlayerName);
}

bool UUI_MainMenu_InGame::InspectPlayerData()
{
	if (!UFVMGameInstance::GetFVMGameInstance())
		return false;

	UGameConfigManager* Config = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager();
	if (!Config)
		return false;

	//先加载选择的角色存档
	if (!UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(this->M_CurrentPlayerDataName.ToString()))
	{
		//如果没有加载成功则加载默认存档
		Config->M_CurrentPlayerName = FString("");
		UFVMGameInstance::GetFVMGameInstance()->InitPlayerStructManager();
		return false;
	}

	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//进行版本匹配
	if (!PlayerData->M_GameVersion.Equals(UFVMGameInstance::GetGameVersion()))
	{

		//设置游戏更新
		UFVMGameInstance::SetUpdateGame();

		//创建弹窗
		UUserWidget* Widget = CreateWidget<UUserWidget>(this, LoadClass<UUserWidget>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_UpdateGameVersion.BPUI_UpdateGameVersion_C'")));
		if (Widget)
			Widget->AddToViewport();
	}


	return true;
}
