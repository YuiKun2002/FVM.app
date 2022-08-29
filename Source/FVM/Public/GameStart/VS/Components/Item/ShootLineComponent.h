// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "ShootLineComponent.generated.h"

class AFlipbookActorBase;

//设射击方向
UENUM(BlueprintType)
enum class EShootDirection : uint8 {
	_EUp  UMETA(DisplayName = "Up"),
	_EDown UMETA(DisplayName = "Down"),
	_ELeft UMETA(DisplayName = "Left"),
	_ERight UMETA(DisplayName = "Right"),
};

//节点
USTRUCT(BlueprintType)
struct FTargetNode {
	GENERATED_USTRUCT_BODY()
public:
	//移动方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EShootDirection M_EShootDirection;
	//可击打类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineType> M_AttackType;
	//移动目标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Target;
	//移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Speed = 15.f;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UShootLineComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//射击移动点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FTargetNode> M_MoveNode;
	//节点位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_NodePosition = 0;
	//目标点
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Target = 0;
	//拥有者
	UPROPERTY(EditInstanceOnly)
		AFlipbookActorBase* M_Owner = nullptr;
public:
	// Sets default values for this component's properties
	UShootLineComponent();

	//击中是否成功
	UFUNCTION(BlueprintCallable)
		bool Hit(const ELineType& _EMouseType);

	//结束射击
	UFUNCTION(BlueprintCallable)
		void Over();

	//移动
	UFUNCTION(BlueprintCallable)
		void Move(const float& X, const float& Y, const float& Z);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
