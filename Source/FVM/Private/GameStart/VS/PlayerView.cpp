// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/PlayerView.h"

#include <Components/BoxComponent.h>
#include <Components/WidgetInteractionComponent.h>

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Sound/SoundBase.h>

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

APlayerView* APlayerView::M_APlayerView = nullptr;

// Sets default values
APlayerView::APlayerView()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->M_UBoxComponent = this->CreateDefaultSubobject<UBoxComponent>(TEXT("M_UBoxComponent"));
	this->M_UCameraComponent = this->CreateDefaultSubobject<UCameraComponent>(TEXT("M_UCameraComponent"));
	this->M_USpringArmComponent = this->CreateDefaultSubobject<USpringArmComponent>(TEXT("M_USpringArmComponent"));
	this->M_UWidgetInteractionComponent = this->CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("M_UWidgetInteractionComponent"));
	this->M_UAudioComponent = this->CreateDefaultSubobject<UAudioComponent>(TEXT("M_UAudioComponent_"));


	this->SetRootComponent(this->M_UBoxComponent);
	this->M_USpringArmComponent->SetupAttachment(this->M_UBoxComponent);
	this->M_UCameraComponent->SetupAttachment(this->M_USpringArmComponent);
}

// Called when the game starts or when spawned
void APlayerView::BeginPlay()
{
	Super::BeginPlay();

	APlayerView::M_APlayerView = this;
	UE_LOG(LogTemp, Warning, TEXT("[APlayerView::BeginPlay] 赋值静态变量M_APlayerView"));
}

void APlayerView::MoveUp(float _Value)
{
	if (_Value != 0.f)
	{
		this->AddActorLocalOffset(FVector(0.f, 0.f, _Value * this->M_PlayerViewMoveSpeed));
	}
}

void APlayerView::MoveRight(float _Value)
{
	if (_Value != 0.f)
	{
		this->AddActorLocalOffset(FVector(0.f, _Value * this->M_PlayerViewMoveSpeed, 0.f));
	}
}

void APlayerView::UpView(float _Value)
{
	if (_Value != 0.f)
	{
		this->M_UCameraComponent->SetFieldOfView(this->M_UCameraComponent->FieldOfView + _Value);
	}
}

// Called every frame
void APlayerView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("PlayerViewMoveUp", this, &APlayerView::MoveUp);
	PlayerInputComponent->BindAxis("PlayerViewMoveRight", this, &APlayerView::MoveRight);

	PlayerInputComponent->BindAxis("PlayerViewUp", this, &APlayerView::UpView);
}

void APlayerView::EnableTouch()
{
	this->M_UWidgetInteractionComponent->InteractionDistance = 2000.f;
}

void APlayerView::DisEnableTouch()
{
	this->M_UWidgetInteractionComponent->InteractionDistance = 0.f;
}

APlayerView* APlayerView::GetPlayerViewInstance()
{
	return APlayerView::M_APlayerView;
}

UAudioComponent* APlayerView::GetAudio()
{
	return this->M_UAudioComponent;
}

void APlayerView::PlayBGM(const FString& _MusicName, bool _bLoop)
{

	//如果正在播放，那么判断正在播放的音频是否一样如果一样则不改变
	if (_MusicName.Equals(this->M_CurrentPlaySoundName))
	{
		UE_LOG(LogTemp, Error, TEXT("[APlayerView::PlayBGM] 音频组件正在播放"));
		return;
	}

	const FString& LSoundPath = FString("SoundWave'/Game/Resource/BP/Audio/GameBG/") + _MusicName + FString(".") + _MusicName + FString("'");

	USoundBase* M_USoundBase = LoadObject<USoundBase>(0, *LSoundPath);

	Cast<USoundWave>(M_USoundBase)->bLooping = _bLoop;

	this->GetAudio()->SetSound(M_USoundBase);

	this->GetAudio()->Play();

	this->M_CurrentPlaySoundName = _MusicName;
}

