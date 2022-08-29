// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "CardProjectionComponent.generated.h"

class ACardActor;
class UPaperFlipbook;

UENUM(BlueprintType)
enum class FCardState : uint8 {
	_EIdle UMETA(DIsplayName = "Idle"),
	_EAttack UMETA(DIsplayName = "Attack"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardProjectionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//默认动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//攻击动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Attack = nullptr;
	//卡片的投射物体
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		AFlipbookActorBase* M_AFlipbookActorBase = nullptr;
	//卡片拥有者
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		ACardActor* M_Owner = nullptr;
	//卡片的状态
	UPROPERTY(EditInstanceOnly, Category = CardState)
		FCardState M_FCardState;
public:
	//默认状态的动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_Anim_Idle;
	//攻击动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_Anim_Attack;
	//卡片投射物->Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_CardActor_Projection_Item;

	//卡片可以攻击的类型(可以攻击 陆地 水上 水下)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		TArray<ELineType> M_CardAttackType;

	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		int32 M_ATK = 5;

	//是否连续投射
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		bool IsContinuousProjection = false;

	//一共发射多少次
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		int32 M_ItemCount = 1;

	//攻击延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_AttackDelay = 3.5f;

	//首次投射时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_FirstProjectionDelay = 1.f;

	//投射时间间隔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float ProjectionDelay = 0.3f;
public:
	// Sets default values for this component's properties
	UCardProjectionComponent();
	//资源加载
	UFUNCTION(BlueprintCallable)
		void LoadResource();
	//攻击
	UFUNCTION(BlueprintCallable)
		void Attack(const float& DeltaTime, const FVector& _Location = FVector::ZeroVector);
	//生成投射物
	UFUNCTION(BlueprintCallable)
		void SpawnCardItem(const float& DeltaTime);
	//更新攻击动画
	UFUNCTION(BlueprintCallable)
		void UpdateAttackAnimation();
	//初始化所有条件
	UFUNCTION(BlueprintCallable)
		void InitCondition();
private:
	//时间记录
	float LTime = 0.f;
	//生成投射物的延迟
	float LSpawnTime = 0.f;
	//攻击模式
	bool InAttack = false;
	//开启投射物发射
	bool InProjectionItem = false;
	//首次投射物是否结束
	bool FirstProjectionEnd = false;
	//投射物生成个数
	int32 SpawnItem = 0;
	//射线设置数据
	TArray<FLineTraceSetting> M_LineSettings;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//生成投射物并且设置属性
	void SpawnCardItems();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
