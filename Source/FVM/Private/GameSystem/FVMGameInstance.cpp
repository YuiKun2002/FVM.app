// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include "GameSystem/MouseStructManager.h"

#include "Game/UI/UI_MainFramework.h"

//地图数据-老鼠回合数据表
#include "Data/MapData/MapDataStruct.h"

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>

//初始化静态游戏实例
UFVMGameInstance* UFVMGameInstance::M_StaticUFVMGameInstance = nullptr;
//主要的显示框架
UUI_MainFramework* UFVMGameInstance::M_UUI_MainFramework = nullptr;

int32 UFVMGameInstance::M_PlayTimes = 1;

UFVMGameInstance::UFVMGameInstance()
{

}

void UFVMGameInstance::Init()
{
	Super::Init();


	UFVMGameInstance::M_StaticUFVMGameInstance = this;
	UE_LOG(LogTemp, Log, TEXT("加载静态UFVMGameInstance"));


	//加载游戏配置
	this->LoadSaveDataToGameConfigManager();
	//初始化角色存档
	this->InitPlayerStructManager();
	//初始化默认地图配置
	this->InitGameMapConfigManager();
	//初始化老鼠配置
	this->InitGameMouseStructManager();
}

void UFVMGameInstance::Shutdown()
{
	Super::Shutdown();

	//保存配置
	if (this->SaveGameConfigManager(this->M_GameConfigManager))
		UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
}

UGameConfigManager* UFVMGameInstance::CreateGameConfigManager()
{
	//判断文件是否存在
	if (this->IsFileExist("GameDataConfig"))
	{
		UE_LOG(LogTemp, Log, TEXT("加载 GameDataConfig"));
		//加载并且返回
		return Cast<UGameConfigManager>(UGameplayStatics::LoadGameFromSlot("GameDataConfig", 0));
	}

	//文件不存在创建SaveGame实例
	UGameConfigManager* _UGameConfigManager = Cast<UGameConfigManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UGameConfigManager>(0, TEXT("Class'/Script/FVM.GameConfigManager'"))));
	//保存
	this->SaveDataToGameConfigManager(_UGameConfigManager);
	//返回
	return _UGameConfigManager;
}

bool UFVMGameInstance::SaveGameConfigManager(UGameConfigManager* const _ConfigInstance)
{
	return UGameplayStatics::SaveGameToSlot(_ConfigInstance, "GameDataConfig", 0);
}

void UFVMGameInstance::InitPlayerStructManager()
{
	//创建一个默认的角色结构管理实例对象并且保存
	if (!this->IsFileExist("DefPlayerStructManager"))
	{
		this->M_UPlayerStructManager = Cast<UPlayerStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UPlayerStructManager>(0, TEXT("Class'/Script/FVM.PlayerStructManager'"))));
		UGameplayStatics::SaveGameToSlot(this->M_UPlayerStructManager, "DefPlayerStructManager", 0);
		UE_LOG(LogTemp, Log, TEXT("保存默认角色结构管理器并加载"));
	}
	else {
		if (!this->M_GameConfigManager->M_CurrentPlayerName.Equals(""))
		{
			this->LoadPlayerStructConfig(this->M_GameConfigManager->M_CurrentPlayerName);
			UE_LOG(LogTemp, Log, TEXT("加载%s角色结构管理器"), *this->M_GameConfigManager->M_CurrentPlayerName);
		}
		else {
			//如果玩家没有建立任何存档则使用默认的存档
			this->M_UPlayerStructManager = Cast<UPlayerStructManager>(this->LoadSaveGame("DefPlayerStructManager"));
			UE_LOG(LogTemp, Log, TEXT("加载默认角色结构管理器"));
		}
	}
}

void UFVMGameInstance::InitGameMapConfigManager()
{
	//保存一个默认的地图管理(前提是默认配置不存在)
	if (!this->IsFileExist("DefGameMapConfig_GameMap"))
	{
		this->CreateGameMapStructManager("DefGameMapConfig");
		//初始化默认地图基本数据
		this->M_UGameMapStructManager->M_Data.M_FirstRowMesheLocation = FVector2D(-135.f, 260.f);
		this->M_UGameMapStructManager->M_Data.M_ColMesheCount = 9;
		this->M_UGameMapStructManager->M_Data.M_RowMesheCount = 6;
		this->M_UGameMapStructManager->M_Data.M_EnableColMesheCount = 9;
		this->M_UGameMapStructManager->M_Data.M_EnableRowMesheCount = 6;

		if (UGameplayStatics::SaveGameToSlot(this->M_UGameMapStructManager, "DefGameMapConfig_GameMap", 0))
		{
			UE_LOG(LogTemp, Log, TEXT("保存默认地图配置管理器"));
		}
	}
	else {
		//加载地图
		this->LoadGameMapStructManager("DefGameMapConfig");

		if (this->GetGameMapStructManager())
			UE_LOG(LogTemp, Log, TEXT("加载默认地图配置管理器"));
	}

	//初始化行间距
	if (this->M_UGameMapStructManager->M_Data.M_RowMargin.Num() != this->M_UGameMapStructManager->M_Data.M_RowMesheCount)
		this->M_UGameMapStructManager->M_Data.M_RowMargin.Init(this->M_UGameMapStructManager->M_Data.M_DefRowUniformMargin, this->M_UGameMapStructManager->M_Data.M_RowMesheCount);
}

void UFVMGameInstance::InitGameMouseStructManager()
{
	if (!this->IsFileExist("DefGameMouseStructConfig_MouseRound"))
	{
		this->CreateMouseStructManager(FString("DefGameMouseStructConfig"));
		//初始化第一个回合
		FMouseRound L_Round_0;
		FMouseRound L_Round_1;
		FMouseRound L_Round_2;
		FMouseRound L_Round_3;
		//设置第一个回合
		L_Round_0.M_RoundBeginDelay = 10.f;
		L_Round_0.M_SpawnTime = 8.f;
		L_Round_0.M_NextRoundTime = 5.f;
		L_Round_0.M_RoundEndDelay = 2.f;
		L_Round_0.M_MouseMaxCount = 1;
		L_Round_0.M_MouseMax = 5;
		//设置第二回合
		L_Round_1.M_SpawnTime = 1.f;
		L_Round_1.M_RoundBeginDelay = 2.f;
		//设置第三回合
		L_Round_2.M_MouseMax = 70;
		L_Round_2.M_SpawnTime = 1.f;
		L_Round_2.M_MouseMaxCount = 15;
		L_Round_2.Key = true;
		//设置第四回合
		L_Round_3.M_MouseMax = 150;
		L_Round_3.M_SpawnTime = 5.f;
		L_Round_3.M_MouseMaxCount = 30;
		//线路
		TArray<int32> Line = { 0,1,2,3,4 };
		L_Round_3.M_MouseLineSetting.Emplace(FMouseLineSetting("Mouse_B_DongJun", Line));
		L_Round_3.M_IsEnableMouseCountZero = true;
		//添加回合
		this->M_MouseStructManager->M_MouseRound.Emplace(L_Round_0);
		this->M_MouseStructManager->M_MouseRound.Emplace(L_Round_1);
		this->M_MouseStructManager->M_MouseRound.Emplace(L_Round_2);
		this->M_MouseStructManager->M_MouseRound.Emplace(L_Round_3);

		if (UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, "DefGameMouseStructConfig_MouseRound", 0))
		{
			UE_LOG(LogTemp, Log, TEXT("保存默认老鼠配置管理器"));
		}
	}
	else {
		this->LoadMouseStructManager("DefGameMouseStructConfig");
		if (this->GetMouseStructManager())
			UE_LOG(LogTemp, Log, TEXT("加载默认老鼠配置管理器"));
	}
}

FString UFVMGameInstance::GetGameVersion()
{
	return UFVMGameInstance::GetFVMGameInstance()->M_GameVersion;
}

void UFVMGameInstance::SetUpdateGame()
{
	UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = true;
}

void UFVMGameInstance::SetGameVersion(FString _Version)
{
	UFVMGameInstance::GetFVMGameInstance()->M_GameVersion = _Version;
}

bool UFVMGameInstance::GetUpdateGame()
{
	return UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate;
}

void UFVMGameInstance::UpdateGameData()
{
	//获取存档
	UPlayerStructManager* PlayerData = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager();

	//更新卡片
	for (auto& Item : PlayerData->M_PlayerItems_Card)
	{
		int32 _CardGrade = Item.M_CardGrade;
		UPlayerStructManager::SearchCardFromDataTable(Item.ItemName, Item, true, Item.M_ECardType);
		//更新卡片等级
		Item.M_CardGrade = _CardGrade;
	}

	//更新材料
	for (auto& Item : PlayerData->M_PlayerItems_Material)
	{
		int32 _ItemCound = Item.M_Count;
		UPlayerStructManager::SearchMaterailFromDataTable(Item.ItemName, Item, true, Item.M_MaterialType);
		//更新材料个数
		Item.M_Count = _ItemCound;
	}

	//更新装备
	for (auto& Item : PlayerData->M_PlayerItems_Equipment)
	{
		int32 _ItemCount = Item.M_IsOverlap ? Item.M_Count : 0;
		bool _Use = Item.M_Used;
		bool _WaitRemove = Item.bWaitRemove;
		UPlayerStructManager::SearchEquipmentFromDataTable(Item.ItemName, Item);
		//更新材料个数
		Item.M_Count = _ItemCount;
		//更新使用情况
		Item.M_Used = _Use;
		//是否等待删除
		Item.bWaitRemove = _WaitRemove;
	}

	//清理掉等待删除的物品
	ClearWaitingItems(PlayerData->M_PlayerItems_Equipment);

	PlayerData->M_GameVersion = UFVMGameInstance::GetGameVersion();

	UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
	UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);

	UFVMGameInstance::GetFVMGameInstance()->M_bGameUpdate = false;
}

UGameMapStructManager* UFVMGameInstance::LoadGameMapStructManager(const FString& _FileName, bool _FromGameMapData)
{
	//从本地加载
	if (!_FromGameMapData)
	{
		//加载了配置之后自动将原来的配置指向新的配置（加载地图和老鼠回合数据）
		UGameMapStructManager* _NewLoad = Cast<UGameMapStructManager>(this->LoadSaveGame(_FileName + FString("_GameMap")));

		if (_NewLoad)
		{
			this->M_UGameMapStructManager = _NewLoad;

			//赋值老鼠回合
			this->M_MouseStructManager = Cast<UMouseStructManager>(this->LoadSaveGame(_NewLoad->M_Data.M_MouseStructConfigName + FString("_MouseRound")));;

			return _NewLoad;
		}
	}


	//勾选从数据表中获取
	if (_FromGameMapData)
	{
		UDataTable* GameMapData = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/MapData/GameMapData.GameMapData'"));
		TArray<FGameMapData*> M_GameMapData;
		GameMapData->GetAllRows("GameMapList", M_GameMapData);

		for (auto Datas : M_GameMapData)
		{
			//从数据表寻找指定的地图名称
			if ((*Datas).M_FGameMapStruct.M_MapName.Equals(_FileName))
			{
				//创建新的地图结构并且将数据表的数据赋予给新创建的地图结构
				this->M_UGameMapStructManager = this->CreateGameMapStructManager(_FileName);
				this->M_UGameMapStructManager->M_Data = (*Datas).M_FGameMapStruct;
				//创建新的老鼠结构并且把数据表的数据赋予给新的老鼠结构
				this->M_MouseStructManager = this->CreateMouseStructManager(_FileName);
				this->M_MouseStructManager->M_MouseRound = (*Datas).M_FMouseRounds.M_MouseRound;

				//保存地图数据
				UGameplayStatics::SaveGameToSlot(this->M_UGameMapStructManager, FString(_FileName + FString("_GameMap")), 0);
				UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, FString(_FileName + FString("_MouseRound")), 0);

				//返回数据
				return this->M_UGameMapStructManager;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("地图配置管理器加载失败!"));
	return nullptr;
}

UGameMapStructManager* UFVMGameInstance::CreateGameMapStructManager(const FString& _FileName, bool _FromGameMapData)
{
	this->M_UGameMapStructManager = Cast<UGameMapStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UGameMapStructManager>(0, TEXT("Class'/Script/FVM.GameMapStructManager'"))));
	UGameplayStatics::SaveGameToSlot(this->M_UGameMapStructManager, *(_FileName + FString("_GameMap")), 0);
	return this->M_UGameMapStructManager;
}

UMouseStructManager* UFVMGameInstance::CreateMouseStructManager(const FString& _FileName, bool _FromGameMapData)
{
	this->M_MouseStructManager = Cast<UMouseStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UMouseStructManager>(0, TEXT("Class'/Script/FVM.MouseStructManager'"))));
	UGameplayStatics::SaveGameToSlot(this->M_MouseStructManager, *(_FileName + FString("_MouseRound")), 0);
	return this->M_MouseStructManager;
}

UMouseStructManager* UFVMGameInstance::LoadMouseStructManager(const FString& _FileName, bool _FromGameMapData)
{
	//加载了配置之后自动将原来的配置指向新的配置
	UMouseStructManager* _NewLoad = Cast<UMouseStructManager>(this->LoadSaveGame(_FileName + FString("_MouseRound")));

	if (_NewLoad)
	{
		this->M_MouseStructManager = _NewLoad;
		return _NewLoad;
	}

	UE_LOG(LogTemp, Log, TEXT("老鼠配置管理器加载失败!"));

	return nullptr;
}

bool UFVMGameInstance::LoadPlayerStructConfig(const FString& _FileName)
{
	UPlayerStructManager* PlayerInstance = Cast<UPlayerStructManager>(LoadSaveGame(_FileName + FString("P")));

	if (PlayerInstance)
	{
		this->M_UPlayerStructManager = PlayerInstance;
		return true;
	}

	return false;
}

UGameConfigManager* UFVMGameInstance::GetGameConfigManager()
{
	return this->M_GameConfigManager;
}

UGameMapStructManager* UFVMGameInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

UPlayerStructManager* UFVMGameInstance::GetPlayerStructManager()
{
	return this->M_UPlayerStructManager;
}

UGameConfigManager* UFVMGameInstance::LoadSaveDataToGameConfigManager()
{
	//获取游戏配置文件
	this->M_GameConfigManager = this->CreateGameConfigManager();

	return this->M_GameConfigManager;
}

void UFVMGameInstance::SaveDataToGameConfigManager(UGameConfigManager* _UGameConfigManager)
{
	if (_UGameConfigManager)
	{
		//保存数据
		if (this->SaveGameConfigManager(_UGameConfigManager))
		{
			UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
		}
	}
	else if (this->M_GameConfigManager)
	{
		//保存数据
		if (this->SaveGameConfigManager(this->M_GameConfigManager))
		{
			UE_LOG(LogTemp, Warning, TEXT("GameDataConfig 数据保存成功"));
		}
	}
}

void UFVMGameInstance::CreatePlayerStructConfig(const FString& _PlayerDataName)
{
	this->M_UPlayerStructManager = Cast<UPlayerStructManager>(UGameplayStatics::CreateSaveGameObject(LoadClass<UPlayerStructManager>(0, TEXT("Class'/Script/FVM.PlayerStructManager'"))));
	this->SavePlayerStructConfig(_PlayerDataName);
}

void UFVMGameInstance::SavePlayerStructConfig(const FString& _PlayerDataName)
{
	UGameplayStatics::SaveGameToSlot(this->M_UPlayerStructManager, _PlayerDataName + FString("P"), 0);
}

void UFVMGameInstance::DeletePlayerStructConfig(const FString& _PlayerDataName)
{
	UGameplayStatics::DeleteGameInSlot(_PlayerDataName + FString("P"), 0);
}

void UFVMGameInstance::PlayBGM(const FString& _MusicName)
{
	//如果组件存在那么先停止播放
	if (this->M_UUAudioComponent)
		this->M_UUAudioComponent->Stop();

	if (_MusicName.Equals(""))
		return;

	const FString& LSoundPath = FString("SoundWave'/Game/Resource/BP/Audio/GameBG/") + _MusicName + FString(".") + _MusicName + FString("'");
	USoundBase* Sound = LoadObject<USoundBase>(0, *LSoundPath);

	if (!Sound)
		return;

	Cast<USoundWave>(Sound)->bLooping = true;
	this->M_UUAudioComponent = UGameplayStatics::SpawnSound2D(this, Sound);
	this->UpdateBGMVolume();
}

void UFVMGameInstance::PlayBGM_S(const FString& _MusicName, const FString& _MusicPath)
{
	if (!this->M_GameConfigManager)
		return;

	if (_MusicName.Equals(""))
		return;

	const FString& LSoundPath = FString("SoundWave'/Game/Resource/BP/Audio/") + _MusicPath + ("/") + _MusicName + FString(".") + _MusicName + FString("'");

	USoundBase* Sound = LoadObject<USoundBase>(0, *LSoundPath);

	if (!Sound)
		return;

	Cast<USoundWave>(Sound)->Volume = this->M_GameConfigManager->M_FMusic.M_BGM_S_Value;
	Cast<USoundWave>(Sound)->bLooping = false;

	UGameplayStatics::PlaySound2D(this, Sound);
}

void UFVMGameInstance::UpdateBGMVolume()
{
	if (this->M_UUAudioComponent && this->M_GameConfigManager)
		this->M_UUAudioComponent->SetVolumeMultiplier(this->M_GameConfigManager->M_FMusic.M_BGM_Value);
}

UMouseStructManager* UFVMGameInstance::GetMouseStructManager()
{
	return this->M_MouseStructManager;
}

UAudioComponent* UFVMGameInstance::GetAudioComponent()
{
	return this->M_UUAudioComponent;
}

void UFVMGameInstance::SetGameVSTimes(const int32& _Value)
{
	if (_Value < 1 || _Value > 5)
	{
		UFVMGameInstance::M_PlayTimes = 1;
	}

	UFVMGameInstance::M_PlayTimes = _Value;
}

const int32& UFVMGameInstance::GetGameVSTimes()
{
	return UFVMGameInstance::M_PlayTimes;
}

void UFVMGameInstance::PrintGameMapStruct(UGameMapStructManager* _UGameMapStructManager)
{
	if (!_UGameMapStructManager)
		return;

	UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UGameMapStructManager-----------------------------------"));
	UE_LOG(LogTemp, Warning, TEXT("多少列网格:%d"), _UGameMapStructManager->M_Data.M_ColMesheCount);
	UE_LOG(LogTemp, Warning, TEXT("启用了多少列网格:%d"), _UGameMapStructManager->M_Data.M_EnableColMesheCount);
	UE_LOG(LogTemp, Warning, TEXT("多少行网格:%d"), _UGameMapStructManager->M_Data.M_RowMesheCount);
	UE_LOG(LogTemp, Warning, TEXT("启用了多少行网格:%d"), _UGameMapStructManager->M_Data.M_EnableRowMesheCount);
	UE_LOG(LogTemp, Warning, TEXT("最大列网格数量:%d"), (uint8)_UGameMapStructManager->M_Data.M_MesheColMax);
	UE_LOG(LogTemp, Warning, TEXT("最大行网格数量:%d"), (uint8)_UGameMapStructManager->M_Data.M_MesheRowMax);
	UE_LOG(LogTemp, Warning, TEXT("第一行网格的坐标: X: %.2f  Y:%.2f "), _UGameMapStructManager->M_Data.M_FirstRowMesheLocation.X, _UGameMapStructManager->M_Data.M_FirstRowMesheLocation.Y);
	UE_LOG(LogTemp, Warning, TEXT("列间距:%.2f"), _UGameMapStructManager->M_Data.M_ColMargin);
	UE_LOG(LogTemp, Warning, TEXT("默认统一行间距:%.2f"), _UGameMapStructManager->M_Data.M_DefRowUniformMargin);
	for (int32 Row = 0; Row < _UGameMapStructManager->M_Data.M_RowMargin.Num(); Row++)
		UE_LOG(LogTemp, Warning, TEXT("第%d行间距:%.2f"), (Row + 1), _UGameMapStructManager->M_Data.M_RowMargin[Row]);

	UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UGameMapStructManager-----------------------------------"));
}

void UFVMGameInstance::PrintGameMouseStruct(UMouseStructManager* _UMouseStructManager)
{
	if (_UMouseStructManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UMouseStructManager-----------------------------------"));
		UE_LOG(LogTemp, Warning, TEXT("一共%d个回合"), _UMouseStructManager->M_MouseRound.Num());

		int32 Index = 0;
		//遍历所有回合
		for (auto& Round : _UMouseStructManager->M_MouseRound)
		{
			Index++;
			UE_LOG(LogTemp, Error, TEXT("-"));
			UE_LOG(LogTemp, Warning, TEXT("-----------------------------------第%d回合-----------------------------------"), Index);
			UE_LOG(LogTemp, Warning, TEXT("1.当所有老鼠被消灭之后自动进行下一回合:%s"), Round.M_IsEnableMouseCountZero ? TEXT("开启") : TEXT("关闭"));
			UE_LOG(LogTemp, Warning, TEXT("2.当前回合老鼠细节个数(用于加载的老鼠):%d"), Round.M_MouseLineSetting.Num());
			UE_LOG(LogTemp, Warning, TEXT("3.当前回合一共有多少只老鼠:%d"), Round.M_MouseMax);
			UE_LOG(LogTemp, Warning, TEXT("4.每次生成的老鼠(最小:%d个,最大%d个)"), Round.M_MouseMinCount, Round.M_MouseMaxCount);
			UE_LOG(LogTemp, Warning, TEXT("5.自动进入下一个回合的时间:%.2f秒,(前提是你关闭了第一项)"), Round.M_NextRoundTime);
			UE_LOG(LogTemp, Warning, TEXT("6.(%.2f秒)之后开始初始化当前回合(BeginPlay)"), Round.M_RoundBeginDelay);
			UE_LOG(LogTemp, Warning, TEXT("7.(%.2f秒)之后开始执行当前回合结束(End)"), Round.M_RoundEndDelay);
			UE_LOG(LogTemp, Warning, TEXT("8.回合一开始可以生成老鼠的最大值:%d"), Round.M_RoundBeginSpawnMaxCount);
			UE_LOG(LogTemp, Warning, TEXT("-----------------------------------第%d回合-----------------------------------"), Index);
			UE_LOG(LogTemp, Error, TEXT("-"));
		}

		UE_LOG(LogTemp, Warning, TEXT("-----------------------------------UMouseStructManager-----------------------------------"));
	}
}

UFVMGameInstance* UFVMGameInstance::GetFVMGameInstance()
{
	return UFVMGameInstance::M_StaticUFVMGameInstance;
}

void UFVMGameInstance::CreateMainFramePanel()
{
	UFVMGameInstance::M_UUI_MainFramework = CreateWidget<UUI_MainFramework>(UFVMGameInstance::GetFVMGameInstance()->GetWorld(), LoadClass<UUI_MainFramework>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/BPUI_MainFrame.BPUI_MainFrame_C'")));
	UFVMGameInstance::M_UUI_MainFramework->AddToViewport();
}

void UFVMGameInstance::DestroyMainFramePanel()
{
	if (UFVMGameInstance::M_UUI_MainFramework)
	{
		UFVMGameInstance::M_UUI_MainFramework->RemoveFromParent();
	}
}

void UFVMGameInstance::PlayBGM_Static(const FString& _MusicName)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM(_MusicName);
	}
}

void UFVMGameInstance::PlayBGM_S_Static(const FString& _MusicName, const FString& _MusicPath)
{
	if (UFVMGameInstance::GetFVMGameInstance())
	{
		UFVMGameInstance::GetFVMGameInstance()->PlayBGM_S(_MusicName, _MusicPath);
	}
}

USaveGame* UFVMGameInstance::LoadSaveGame(const FString& _FileName)
{
	return UGameplayStatics::LoadGameFromSlot(_FileName, 0);
}

void UFVMGameInstance::SaveGame(USaveGame* SaveGameObject, const FString& SlotName)
{
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

bool UFVMGameInstance::IsFileExist(const FString& _FileName)
{
	return UGameplayStatics::DoesSaveGameExist(_FileName, 0);
}
