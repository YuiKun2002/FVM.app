// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/MapBase.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameMapStructManager.h"

#include <Paper2D/Classes/PaperSprite.h>

// Sets default values
AMapBase::AMapBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapBase::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("AMapBase"));

	//设置当前地图Actor的位置
	this->SetActorLocation(this->M_MapTranform.M_Location);
	this->SetActorRotation(this->M_MapTranform.M_Rotation);
	this->SetActorScale3D(this->M_MapTranform.M_Scale);

	this->InitMapSprite();
}

void AMapBase::InitMapSprite()
{
	//通过存档获取地图的背景名称
	const FString& _BGName = UFVMGameInstance::GetFVMGameInstance()->GetGameMapStructManager()->M_Data.M_MapBGName;
	const FString& _MapBgPath = FString("PaperSprite'/Game/Resource/Texture/Sprite/VS/Map/") + _BGName + FString(".") + _BGName + FString("'");
	this->M_MapSprite.M_MapBackgroundSprite = LoadObject<UPaperSprite>(0, *_MapBgPath);
}

// Called every frame
void AMapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

