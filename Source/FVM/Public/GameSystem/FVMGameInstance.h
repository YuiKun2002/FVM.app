// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FVMGameInstance.generated.h"

//用于加载存档和地图的各种配置
class UPlayerStructManager;
class UGameMapStructManager;
class UGameConfigManager;
class UMouseStructManager;
class UUI_MainFramework;
class UAudioComponent;
class USaveGame;

UCLASS()
class FVM_API UFVMGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//游戏版本（用于识别存档数据->一旦发现版本不匹配，则更新旧版存档）
	UPROPERTY(EditAnyWhere)
		FString M_GameVersion = "0.0.0.1";
	//游戏是否更新
	UPROPERTY(EditAnyWhere)
		bool M_bGameUpdate = false;
private:
	//游戏静态实例对象
	static UFVMGameInstance* M_StaticUFVMGameInstance;
	static UUI_MainFramework* M_UUI_MainFramework;
	//游戏设置数据
	UPROPERTY(EditAnyWhere)
		UGameConfigManager* M_GameConfigManager = nullptr;
	//游戏地图数据
	UPROPERTY(EditAnyWhere)
		UGameMapStructManager* M_UGameMapStructManager = nullptr;
	//老鼠结构数据
	UPROPERTY(EditAnyWhere)
		UMouseStructManager* M_MouseStructManager = nullptr;
	//角色数据
	UPROPERTY(EditAnyWhere)
		UPlayerStructManager* M_UPlayerStructManager = nullptr;
	//背景音乐组件
	UPROPERTY(EditAnyWhere)
		UAudioComponent* M_UUAudioComponent = nullptr;

	//倍速->比如VS时开启2倍速
	static int32 M_PlayTimes;
protected:
	//创建一个游戏配置管理器的实例
	UGameConfigManager* CreateGameConfigManager();
	//保存一个游戏配置管理器
	bool SaveGameConfigManager(UGameConfigManager* const _ConfigInstance);
public:
	UFVMGameInstance();
	//初始化角色存档
	void InitPlayerStructManager();
	//初始化默认地图配置器存档
	void InitGameMapConfigManager();
	//初始化默认老鼠配置
	void InitGameMouseStructManager();
	//获取游戏版本
	static FString GetGameVersion();
	//设置更新
	static void SetUpdateGame();
	//设置游戏版本
	UFUNCTION(BlueprintCallable)
		static void SetGameVersion(FString _Version);
	//更新游戏数据
	UFUNCTION(BlueprintCallable)
		static void UpdateGameData();
	//获取更新
	UFUNCTION(BlueprintCallable)
		static bool GetUpdateGame();
public:
	//加载一个游戏地图管理器（可以选择是否从数据表中加载数据,会先判断本地是否存在,如果选择true,会从数据表检索，如果成功则会自动创建）
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* LoadGameMapStructManager(const FString& _FileName, bool _FromGameMapData = false);
	//创建一个游戏地图管理器（可以选择是否从数据表中加载数据,会先判断本地是否存在,如果选择true,会从数据表检索，如果成功则会自动创建）
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* CreateGameMapStructManager(const FString& _FileName, bool _FromGameMapData = false);
	//加载一个老鼠配置管理器（可以选择是否从数据表中加载数据,会先判断本地是否存在,如果选择true,会从数据表检索，如果成功则会自动创建）
	UFUNCTION(BlueprintCallable)
		UMouseStructManager* LoadMouseStructManager(const FString& _FileName, bool _FromGameMapData = false);
	//创建一个老鼠配置管理器（可以选择是否从数据表中加载数据,会先判断本地是否存在,如果选择true,会从数据表检索，如果成功则会自动创建）
	UFUNCTION(BlueprintCallable)
		UMouseStructManager* CreateMouseStructManager(const FString& _FileName, bool _FromGameMapData = false);
	//加载角色配置
	UFUNCTION(BlueprintCallable)
		bool LoadPlayerStructConfig(const FString& _FileName);
	//加载游戏配置文件
	UFUNCTION(BlueprintCallable)
		UGameConfigManager* LoadSaveDataToGameConfigManager();
public:
	//获取游戏配置
	UFUNCTION(BlueprintCallable)
		UGameConfigManager* GetGameConfigManager();
	//获取游戏地图配置管理
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* GetGameMapStructManager();
	//获取角色结构管理
	UFUNCTION(BlueprintCallable)
		UPlayerStructManager* GetPlayerStructManager();
	//获取老鼠结构管理
	UFUNCTION(BlueprintCallable)
		UMouseStructManager* GetMouseStructManager();
	//获取背景音乐播放组件
	UFUNCTION(BlueprintCallable)
		UAudioComponent* GetAudioComponent();
public:
	//将游戏配置文件保存
	UFUNCTION(BlueprintCallable)
		void SaveDataToGameConfigManager(UGameConfigManager* _UGameConfigManager = nullptr);

	//--------------------------------------------------[角色数据]-------------------------------------------------------
	//创建角色数据
	UFUNCTION(BlueprintCallable)
		void CreatePlayerStructConfig(const FString& _PlayerDataName);
	//保存角色数据
	UFUNCTION(BlueprintCallable)
		void SavePlayerStructConfig(const FString& _PlayerDataName);
	//删除角色存档
	UFUNCTION(BlueprintCallable)
		void DeletePlayerStructConfig(const FString& _PlayerDataName);

	//播放背景音乐
	UFUNCTION(BlueprintCallable)
		void PlayBGM(const FString& _MusicName);
	//播放音效
	UFUNCTION(BlueprintCallable)
		void PlayBGM_S(const FString& _MusicName, const FString& _MusicPath);
	//更新背景音乐的音量
	UFUNCTION(BlueprintCallable)
		void UpdateBGMVolume();
public:
	//设置游戏开始时的倍速
	UFUNCTION(BlueprintCallable)
		static void SetGameVSTimes(const int32& _Value);
	//获取游戏开始的倍速
	UFUNCTION(BlueprintCallable)
		static const int32& GetGameVSTimes();
public:
	//打印游戏地图结构
	UFUNCTION(BlueprintCallable)
		void PrintGameMapStruct(UGameMapStructManager* _UGameMapStructManager);
	//打印老鼠结构
	UFUNCTION(BlueprintCallable)
		void PrintGameMouseStruct(UMouseStructManager* _UMouseStructManager);
public:
	//获取FVM2游戏实例
	UFUNCTION(BlueprintCallable)
		static UFVMGameInstance* GetFVMGameInstance();
	//创建一个主要的ui显示框架
	UFUNCTION(BlueprintCallable)
		static void CreateMainFramePanel();
	//销毁一个主要的ui显示框架
	UFUNCTION(BlueprintCallable)
		static void DestroyMainFramePanel();
	//播放背景音乐
	UFUNCTION(BlueprintCallable)
		static void PlayBGM_Static(const FString& _MusicName);
	//播放音效
	UFUNCTION(BlueprintCallable)
		static void PlayBGM_S_Static(const FString& _MusicName, const FString& _MusicPath = FString("GameBG"));
public:
	//加载游戏对象
	UFUNCTION(BlueprintCallable)
		USaveGame* LoadSaveGame(const FString& _FileName);
	//保存游戏对象
	UFUNCTION(BlueprintCallable)
		void SaveGame(USaveGame* SaveGameObject, const FString& SlotName);
	//判断文件是否存在
	UFUNCTION(BlueprintCallable)
		bool IsFileExist(const FString& _FileName);
public:
	//删除玩家背包中有待删除的道具
	template<class ItemType>
	static void ClearWaitingItems(TArray<ItemType>& _Arrays)
	{
		TArray<ItemType> _Buff;

		for (ItemType& _Items : _Arrays)
		{
			//将未标记的物品添加到缓冲区
			if (!_Items.bWaitRemove)
				_Buff.Emplace(_Items);
		}
		//清空
		_Arrays.Empty();
		//重新复制新值
		_Arrays = _Buff;
	}
protected:
	//初始化
	virtual void Init() override;
	//关闭
	virtual void Shutdown() override;
};
