// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/VS/MapBaseType.h"
#include "MouseStateMachineComponent.generated.h"

class AMouseActor;
class AMapMeshe;

UENUM(BlueprintType)
enum class EMouseState : uint8 {
	A UMETA(Display = "Def"),
	B UMETA(Display = "2/3"),
	C UMETA(Display = "1/3"),
	D UMETA(Display = "Death")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMouseStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY()
		bool M_bGround = false;
	UPROPERTY()
		bool M_bOnWater = false;
	UPROPERTY()
		bool M_bSky = false;
	UPROPERTY()
		bool M_bUnderWater = false;
	UPROPERTY()
		AMouseActor* M_Owner = nullptr;
private:
	//当前动画播放一次后进行跳转到指定动画
	UPROPERTY()
		bool M_bIsAnimCurrentPlayOnceToJump = false;
	//需要跳转的动画
	UPROPERTY()
		FString M_JumpAnimName;
public:
	//线路类型(一个时间段只能是一种状态类型)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType;

	//老鼠的动画路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_MouseAnimPath;

	//老鼠动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CurrentPlayAnim;

public:
	// Sets default values for this component's properties
	UMouseStateMachineComponent();

	//获取HP的位置 1 2/3  1/3
	float GetHPState(float _TotalHP, float _HP);

	//切换线路
	UFUNCTION(BlueprintCallable)
		bool ChangeLineType(ELineType _LineType);

	//判断当前网格Line和类型是否适合老鼠
	UFUNCTION(BlueprintCallable)
		bool JudgeCurrentMeshLineType(AMapMeshe* _MapMesh);

	//设置老鼠可适应类型
	UFUNCTION(BlueprintCallable)
		void SetMouseType(bool _Ground, bool _OnWater, bool _Sky, bool _UnderWater);

	//设置动画路径
	UFUNCTION(BlueprintCallable)
		void SetAnimPath(const FString& _AnimPath);

	UFUNCTION(BlueprintCallable)
		void PlayAnim(FString _Anim, bool IsPlayOnceToJump = false, FString _JumpAnim = "");

	//获取老鼠状态动画的路径
	UFUNCTION(BlueprintCallable)
		EMouseState GetState(float _TotalHP, float _HP);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
