// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateCardBar.generated.h"

class UWidgetComponent;
class UUI_MapMeshe;

UCLASS()
class FVM_API ACreateCardBar : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		UWidgetComponent* M_UWidgetComponent = nullptr;
public:
	UUI_MapMeshe* M_UI_MapMeshe = nullptr;
public:
	// Sets default values for this actor's properties
	ACreateCardBar();

	void Init();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
