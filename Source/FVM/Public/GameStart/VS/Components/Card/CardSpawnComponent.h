// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CardSpawnComponent.generated.h"

class ACardActor;
class UPaperFlipbook;
class AFlipbookActorBase;

UENUM(BlueprintType)
enum class FCardSpawn : uint8 {
	_EIdle UMETA(DIsplayName = "Idle"),
	_ESpawn UMETA(DIsplayName = "Spawn"),
};

//资源数据
USTRUCT(BlueprintType)
struct FUCardSpawnComponent_Resource {
	GENERATED_USTRUCT_BODY()
public:
	//默认动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//生产动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Spawn = nullptr;
	//生长动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Spawn_Up = nullptr;
};

//条件数据
USTRUCT(BlueprintType)
struct FUCardSpawnComponent_Condition {
	GENERATED_USTRUCT_BODY()
public:
	//当前卡片是否可以生长
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		bool M_CurrentCardIsWeekUp = false;
	//成长延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_AutoUpTime = 0.f;
	//当前记录的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_AutoUpCurrentTime = 0.f;
	//提升火苗产量的倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_SpawnFlameNumRate = 2.f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardSpawnComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//默认动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Idle = nullptr;
	//生产动画
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		UPaperFlipbook* M_UPaperFlipbook_Spawn = nullptr;
	//生产物体
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		AFlipbookActorBase* M_AFlipbookActorBase = nullptr;
	//卡片拥有者
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		ACardActor* M_Owner = nullptr;
	//卡片的状态
	UPROPERTY(EditInstanceOnly, Category = CardState)
		FCardSpawn M_FCardSpawn;

	//条件
	UPROPERTY(EditInstanceOnly, Category = CardState)
		FUCardSpawnComponent_Condition M_Condition;
	//动画资源实例
	UPROPERTY(EditInstanceOnly, Category = ResInstance)
		FUCardSpawnComponent_Resource M_Resource;
public:

	//默认状态的动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_Anim_Idle;

	//生产动画
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_Anim_Spawn;

	//卡片生产物体->Actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Resource)
		FString M_CardActor_Spawn_Item;

	//是否连续生产
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		bool IsContinuousSpawn = false;

	//生产的火苗值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		int32 M_SpawnFlameValue = 25;

	//一共生成多少个
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		int32 M_SpawnCount = 1;

	//生成模式启动延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_SpawnDelay = 3.5f;

	//首次生成时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float M_FirstSpawnDelay = 1.f;

	//首次生成后剩余的物体生成延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CardProperty)
		float SpawnDelay = 0.3f;

private:
	//时间记录
	float LTime = 0.f;
	//生成的延迟
	float LSpawnTime = 0.f;
	//生成模式
	bool InSpawn = false;
	//生长模式
	bool InWeekUp = false;
	//开启物品生成
	bool InSpawnItem = false;
	//首次投射物是否结束
	bool FirstSpawnEnd = false;
	//生成个数
	int32 SpawnItem = 0;

public:
	// Sets default values for this component's properties
	UCardSpawnComponent();

	//资源加载
	UFUNCTION(BlueprintCallable)
		void LoadResource();
	//攻击
	UFUNCTION(BlueprintCallable)
		void Spawn(const float& DeltaTime, const FVector& _Location = FVector::ZeroVector);
	//生成物体
	UFUNCTION(BlueprintCallable)
		void BeginSpawnItem(const float& DeltaTime);
	//更新攻击动画
	UFUNCTION(BlueprintCallable)
		void UpdateSpawnAnimation();
	//初始化所有条件
	UFUNCTION(BlueprintCallable)
		void InitCondition();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//生成投射物并且设置属性
	void SpawnItems();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
