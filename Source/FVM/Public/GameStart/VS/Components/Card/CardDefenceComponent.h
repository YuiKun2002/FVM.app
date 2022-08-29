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
	//1����(Ѫ��)
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//1/3����
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle2 = nullptr;
	//2/3����
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle3 = nullptr;
	//��Ƭӵ����
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

	//����״̬
	UFUNCTION(BlueprintCallable)
		void UpdateState();
	//������Դ
	UFUNCTION(BlueprintCallable)
		void LoadResource();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
