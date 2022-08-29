// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/GameMapInstance.h"

#include "GameSystem/GameMapStructManager.h"

#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/Components/VSManagerComponent.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperSpriteActor.h>
#include <Paper2D/Classes/PaperSpriteComponent.h>

#include "GameSystem/FVMGameInstance.h"

#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>

// Sets default values
AGameMapInstance::AGameMapInstance()
{
	PrimaryActorTick.bCanEverTick = true;

	//网格控制管理
	this->M_MesheControllComponent = this->CreateDefaultSubobject<UMesheControllComponent>("_UMesheControllComponent");
	
	/*

	//VS管理
	this->M_VSManagerComponent = this->CreateDefaultSubobject<UVSManagerComponent>("_UVSManagerComponent");
	//卡片管理
	this->M_CardManagerComponent = this->CreateDefaultSubobject<UCardManagerComponent>("_UCardManagerComponent");;
	//老鼠管理
	this->M_MouseManagerComponent = this->CreateDefaultSubobject<UMouseManagerComponent>("_UMouseManagerComponent");;
	//资源管理
	this->M_ResourceManagerComponent = this->CreateDefaultSubobject<UResourceManagerComponent>("_UResourceManagerComponent");

	*/

}

//初始化静态变量
AGameMapInstance* AGameMapInstance::M_AGameMapInstance = nullptr;

AGameMapInstance* AGameMapInstance::GetGameMapInstance()
{
	return AGameMapInstance::M_AGameMapInstance;
}

void AGameMapInstance::CreatePaperSpriteActorToWorld(APaperSpriteActor* _Actor, UPaperSprite* _PSprite, const FTransform& _ActorTranform)
{
	if (!_Actor)
		return;

	APaperSpriteActor* LAMapBackground = this->GetWorld()->SpawnActor<APaperSpriteActor>(_Actor->GetClass(), _ActorTranform);
	LAMapBackground->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	LAMapBackground->GetRenderComponent()->SetSprite(_PSprite);
}

UMesheControllComponent* AGameMapInstance::GetMesheControllComponent()
{
	return this->M_MesheControllComponent;
}

void AGameMapInstance::InitGameMapStructManager(UGameMapStructManager* _UGameMapStructManager)
{
	this->M_UGameMapStructManager = _UGameMapStructManager;
}

UGameMapStructManager* AGameMapInstance::GetGameMapStructManager()
{
	return this->M_UGameMapStructManager;
}

void AGameMapInstance::PlaySound(const FString& _MusicName)
{
	//如果正在播放，那么判断正在播放的音频是否一样如果一样则不改变
	if (_MusicName.Equals(this->M_CurrentPlaySoundName))
		return;

	UFVMGameInstance::GetFVMGameInstance()->PlayBGM(_MusicName);

	this->M_CurrentPlaySoundName = _MusicName;
}


void AGameMapInstance::BeginPlay()
{
	Super::BeginPlay();
	//初始化静态变量
	this->InitStaticGameMapInstance();
	//初始化背景
	this->InitGameMapBackground();
}

void AGameMapInstance::InitGameMapBackground()
{
	//创建背景
	this->M_AMapBackground = NewObject<APaperSpriteActor>(this->GetWorld(), "M_AMapBackground");
	this->CreatePaperSpriteActorToWorld(this->M_AMapBackground, this->M_MapSprite.M_MapBackgroundSprite, this->GetActorTransform());
}

// Called every frame
void AGameMapInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameMapInstance::GameStart()
{
	this->IsInGame = true;
	this->M_VSManagerComponent->GameStart();
	this->M_CardManagerComponent->GameStart();
	this->M_MouseManagerComponent->GameStart();
	this->M_ResourceManagerComponent->GameStart();

	//音乐初始化
	if (this->M_UGameMapStructManager)
		this->PlaySound(this->M_UGameMapStructManager->M_Data.M_BeginMusic);

}

void AGameMapInstance::GameOver()
{
	this->IsInGame = false;
	this->M_VSManagerComponent->GameOver();
	this->M_CardManagerComponent->GameOver();
	this->M_MouseManagerComponent->GameOver();
	this->M_ResourceManagerComponent->GameOver();
}

bool AGameMapInstance::InitStaticGameMapInstance()
{
	//判断当前变量是否有效
	AGameMapInstance::M_AGameMapInstance = this;
	return true;
}
