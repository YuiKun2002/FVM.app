// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerView.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;
class UWidgetInteractionComponent;
class UAudioComponent;

UCLASS()
class FVM_API APlayerView : public APawn
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "PlayerView")
		USpringArmComponent* M_USpringArmComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "PlayerCollision")
		UBoxComponent* M_UBoxComponent = nullptr;

	//静态角色控制器
	static APlayerView* M_APlayerView;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerView")
		UWidgetInteractionComponent* M_UWidgetInteractionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerView")
		UCameraComponent* M_UCameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAudio")
		UAudioComponent* M_UAudioComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerViewMove")
		float M_PlayerViewMoveSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAudio_Name")
		FString M_CurrentPlaySoundName;
public:
	// Sets default values for this pawn's properties
	APlayerView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void MoveUp(float _Value);

	UFUNCTION()
		void MoveRight(float _Value);

	UFUNCTION()
		void UpView(float _Value);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void EnableTouch();

	UFUNCTION(BlueprintCallable)
		void DisEnableTouch();

	UFUNCTION(BlueprintCallable)
		UAudioComponent* GetAudio();

	//播放背景音乐
	UFUNCTION(BlueprintCallable)
		void PlayBGM(const FString& _MusicName, bool _bLoop = true);

	UFUNCTION(BlueprintCallable)
		static APlayerView* GetPlayerViewInstance();
};
