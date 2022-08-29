// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardDefenceComponent.generated.h"

class ACardActor;
class UPaperFlipbook;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardDefenceComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//1动画(血量)
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//1/3动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle2 = nullptr;
	//2/3动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle3 = nullptr;
	//卡片拥有者
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		ACardActor* M_Owner = nullptr;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_AnimIdle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_AnimIdle2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_AnimIdle3;
public:
	// Sets default values for this component's properties
	UCardDefenceComponent();

	//更新状态
	UFUNCTION(BlueprintCallable)
		void UpdateState();
	//加载资源
	UFUNCTION(BlueprintCallable)
		void LoadResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
