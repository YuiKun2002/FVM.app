// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "MouseBeheviorComponent.generated.h"

class AMouseActor;
class UFVMGameInstance;
class UMouseManagerComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseBeheviorComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//获取当前组件的拥有者(用来控制坐标生命)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMouseActor* _Owner = nullptr;
	//定时器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimerHandle M_MouseUpdate;
public:
	// Sets default values for this component's properties
	UMouseBeheviorComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//向上移动
	void MoveToUp(const float& _DeltaTime = 0.f);
	//向下移动
	void MoveToBottom(const float& _DeltaTime = 0.f);
	//向左移动
	void MoveToLeft(const float& _DeltaTime = 0.f);
	//向右移动
	void MoveToRight(const float& _DeltaTime = 0.f);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
