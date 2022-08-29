// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/VS/MapBase.h"
#include "GameMapInstance.generated.h"

class APaperSpriteActor;
class UPaperSpriteComponent;
class UGameMapStructManager;
class UMesheControllComponent;
class UVSManagerComponent;
class UCardManagerComponent;
class UMouseManagerComponent;
class UResourceManagerComponent;
class UAudioComponent;

UCLASS()
class FVM_API AGameMapInstance : public AMapBase
{
	GENERATED_BODY()

public:
	//地图背景
	UPROPERTY(EditAnywhere, Category = "Actor | Bg")
		APaperSpriteActor* M_AMapBackground = nullptr;

	//静态地图实例
	static AGameMapInstance* M_AGameMapInstance;
	//地图的配置结构
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Struct")
		UGameMapStructManager* M_UGameMapStructManager = nullptr;
	//格子控制组件
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UMesheControllComponent* M_MesheControllComponent = nullptr;
	//游戏开始VS管理器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UVSManagerComponent* M_VSManagerComponent = nullptr;
	//卡片管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UCardManagerComponent* M_CardManagerComponent = nullptr;
	//老鼠管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UMouseManagerComponent* M_MouseManagerComponent = nullptr;
	//资源管理
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Component")
		UResourceManagerComponent* M_ResourceManagerComponent = nullptr;
public:
	//是否在游戏(将区别与开发与实际游玩)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Condition")
		bool IsInGame = false;
	//当前正在播放的音频名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor | Condition")
		FString M_CurrentPlaySoundName = FString("");
public:
	// Sets default values for this actor's properties
	AGameMapInstance();

	//获取游戏地图实例
	UFUNCTION(BlueprintCallable)
		static AGameMapInstance* GetGameMapInstance();

	//创建一个2D的Actor到世界
	UFUNCTION(BlueprintCallable)
		void CreatePaperSpriteActorToWorld(APaperSpriteActor* _Actor, UPaperSprite* _PSprite, const FTransform& _ActorTranform);

	//获取网格控制组件
	UFUNCTION(BlueprintCallable)
		UMesheControllComponent* GetMesheControllComponent();

	//初始化地图配置管理
	UFUNCTION(BlueprintCallable)
		void InitGameMapStructManager(UGameMapStructManager* _UGameMapStructManager);

	//获取当前地图所使用的地图配置管理器
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* GetGameMapStructManager();
	 
	//播放背景音乐
	UFUNCTION(BlueprintCallable)
		void PlaySound(const FString& _MusicName);
protected:

	virtual void BeginPlay() override;

	//初始化地图的背景
	void InitGameMapBackground();

public:
	virtual void Tick(float DeltaTime) override;
	//初始化静态地图的实例对象
	UFUNCTION(BlueprintCallable)
		bool InitStaticGameMapInstance();
	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
