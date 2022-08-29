// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Card/CardProjectionComponent.h"

#include "GameStart/Flipbook/GameActor/FlyItemActor.h"
#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
// Sets default values for this component's properties
UCardProjectionComponent::UCardProjectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardProjectionComponent::LoadResource()
{
	this->M_Owner = Cast<ACardActor>(this->GetOwner());

	if (!this->M_Owner)
		return;

	int32 ResourcePathCount = this->M_Owner->M_ResourcePath.Num();

	if (ResourcePathCount > 0)
		this->M_UPaperFlipbook_Idle = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[0]);


	if (ResourcePathCount > 1)
		this->M_UPaperFlipbook_Attack = LoadObject<UPaperFlipbook>(nullptr, *this->M_Owner->M_ResourcePath[1]);

	if (ResourcePathCount > 2)
	{
		TSubclassOf<AFlipbookActorBase> L_CardProjectionItem = LoadClass<AFlipbookActorBase>(nullptr, *this->M_Owner->M_ResourcePath[2]);
		this->M_AFlipbookActorBase = L_CardProjectionItem.GetDefaultObject();
	}

	if (!this->M_AFlipbookActorBase)
	{
		UE_LOG(LogTemp, Error, TEXT("卡片投射物加载失败!请检查路径和末尾的\"_C\""));
		return;
	}

	this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);

	//获取攻击速度并且设置值
	this->M_AttackDelay = this->M_Owner->GetCardAttackSpeed();
	this->M_ItemCount = this->M_Owner->GetCardAttackItemCount();
	this->M_FirstProjectionDelay = this->M_Owner->GetCardFirstAttackSpeed();
	this->ProjectionDelay = this->M_Owner->GetCardAttackProjectionSpeed();
	//设置射线偏移起点
	this->M_LineSettings = this->M_Owner->GetLineTraceSetting();
}

void UCardProjectionComponent::Attack(const float& DeltaTime, const FVector& _Location)
{
	//时间累计
	if (!this->InAttack)
		this->LTime += DeltaTime;
	else
		this->LTime = 0.f;

	//攻击模式冷却
	if (LTime >= this->M_AttackDelay)
	{
		this->LTime = 0.f;
		this->LSpawnTime = 0.f;
		this->SpawnItem = this->M_ItemCount;
		this->InAttack = true;
		this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Attack);
	}
}

void UCardProjectionComponent::SpawnCardItem(const float& DeltaTime)
{
	//投射物模式启动
	if (this->InProjectionItem && this->SpawnItem > 0)
	{
		//首次投射物是否已经生成
		if (this->FirstProjectionEnd)
		{
			//首次投射物已经生成完毕，现在是生成剩余投射物
			this->LSpawnTime += DeltaTime;
			if (this->LSpawnTime >= this->ProjectionDelay)
			{
				//生成投射物
				this->SpawnCardItems();
				//时间归零
				this->LSpawnTime = 0;
				//总个数减一
				this->SpawnItem--;
			}
		}
		else {
			//生成首次投射物
			this->LSpawnTime += DeltaTime;
			if (this->LSpawnTime >= this->M_FirstProjectionDelay)
			{
				this->FirstProjectionEnd = true;
				//总个数减一
				this->SpawnItem--;
				//生成投射物
				this->SpawnCardItems();
				//时间归零
				this->LSpawnTime = 0.f;
			}
		}

		//如果所有投射物生成完毕则关闭投射模式
		if (this->SpawnItem == 0)
		{
			this->InProjectionItem = false;
			this->FirstProjectionEnd = false;
		}
	}
}

void UCardProjectionComponent::UpdateAttackAnimation()
{
	//进入攻击模式
	if (InAttack)
	{
		if (this->M_Owner->AnimationIsEnd())
		{
			this->M_Owner->GetRenderComponent()->SetFlipbook(this->M_UPaperFlipbook_Idle);

			//时间记录
			this->LTime = 0.f;
			//生成投射物的延迟
			this->LSpawnTime = 0.f;
			//攻击模式
			this->InAttack = false;
		}

		//开启投射物模式
		this->InProjectionItem = true;
	}
}

void UCardProjectionComponent::InitCondition()
{
	//时间记录
	this->LTime = 0.f;
	//生成投射物的延迟
	this->LSpawnTime = 0.f;
	//攻击模式
	this->InAttack = false;
	//投射物生成个数
	this->SpawnItem = 0;
}


// Called when the game starts
void UCardProjectionComponent::BeginPlay()
{
	Super::BeginPlay();

	this->M_Owner = Cast<ACardActor>(this->GetOwner());

	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("获取卡片Actor失败!"));
		this->SetComponentTickEnabled(false);
		return;
	}
}

void UCardProjectionComponent::SpawnCardItems()
{
	if (!this->M_Owner->M_CurrentMouse)
		return;

	if (this->M_Owner->M_CurrentMouse->IsPendingKillPending())
		return;

	//生成投射物
	AFlyItemActor* const NewSpawn = this->GetWorld()->SpawnActor<AFlyItemActor>(this->M_AFlipbookActorBase->GetClass(), this->M_Owner->GetActorTransform());
	//新生成的对象设置自定义拥有者(CardActor)
	NewSpawn->M_CustomActorOwner = this->M_Owner;
	NewSpawn->M_MouseActorLocation = this->M_Owner->M_CurrentMouse;
	NewSpawn->ATK = this->M_Owner->GetCardATK();
	NewSpawn->_SecondATK = this->M_Owner->GetCardSecondATK();
	NewSpawn->M_Line = this->M_Owner->M_Line.Row;
}

// Called every frame
void UCardProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!this->M_Owner)
		return;

	//卡片休息
	if (!this->M_Owner->GetCardDay() && this->M_Owner->GetMapDay())
		return;

	FHitResult Result;
	FCollisionQueryParams Params;
	FCollisionResponseParams _ResParams;

	for (auto& Line : this->M_LineSettings)
	{
		DrawDebugLine(this->GetWorld(), Line.M_BeginLocation, Line.M_EndLocation, FColor::Red);
		if (this->GetWorld()->LineTraceSingleByChannel(Result, Line.M_BeginLocation, Line.M_EndLocation, ECollisionChannel::ECC_Visibility, Params, _ResParams))
		{
			this->M_Owner->AnalysisActor(Result.GetActor());
		}
	}

	//如果老鼠存在
	if (this->M_Owner->GetCurrentMouse() && !this->M_Owner->GetCurrentMouse()->IsPendingKillPending())
	{
		this->Attack(DeltaTime);
		this->SpawnCardItem(DeltaTime);
	}

	this->UpdateAttackAnimation();
}

