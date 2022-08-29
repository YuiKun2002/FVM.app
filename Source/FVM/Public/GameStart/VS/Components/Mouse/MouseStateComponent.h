// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MouseStateComponent.generated.h"

class AMouseActor;
class UPaperFlipbook;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseStateComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = FlipBookInstance)
		UPaperFlipbook* M_UPaperFlipbook_Def = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = FlipBookInstance)
		UPaperFlipbook* M_UPaperFlipbook_Attack = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = FlipBookInstance)
		UPaperFlipbook* M_UPaperFlipbook_LowDef = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = FlipBookInstance)
		UPaperFlipbook* M_UPaperFlipbook_LowAttack = nullptr;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = FlipBookInstance)
		UPaperFlipbook* M_UPaperFlipbook_Death = nullptr;
public:
	//老鼠的默认动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FlipBookPath)
		FString M_MouseDef;
	//老鼠的攻击动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FlipBookPath)
		FString M_MouseAttack;
	//老鼠的半血默认动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FlipBookPath)
		FString M_MouseLowDef;
	//老鼠的半血攻击动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FlipBookPath)
		FString M_MouseLowAttack;
	//老鼠的死亡动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FlipBookPath)
		FString M_MouseDeath;
	//老鼠
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _MouseActor)
		AMouseActor* M_Owner = nullptr;
public:
	// Sets default values for this component's properties
	UMouseStateComponent();
	//初始化素材
	UFUNCTION(BlueprintCallable)
		void InitResource();
	//更新老鼠状态
	UFUNCTION(BlueprintCallable)
		bool UpdateState(AMouseActor* _MAMouseActor);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
