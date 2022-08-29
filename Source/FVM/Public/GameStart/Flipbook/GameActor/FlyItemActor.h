// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/FlipbookActorBase.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"
#include "FlyItemActor.generated.h"


UENUM(BlueprintType)
enum class Buff_Infor : uint8
{
	//减速
	E_SlowDown,
	//冻结
	E_Freeze,
	//凝固
	E_Solidification,
	//加速
	E_Accelerate,
	//灼烧
	Burn,
};

//发送老鼠的信息
USTRUCT(BlueprintType)
struct FItem_Buff {
	GENERATED_USTRUCT_BODY()
public:
	//条件->是否携带Buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bCondition = false;
	//多少个buff （key（buff） value（time））
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<Buff_Infor, float> M_Buffers;
};

UCLASS()
class FVM_API AFlyItemActor : public AFlipbookActorBase
{
	GENERATED_BODY()
public:
	//老鼠的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* M_MouseActorLocation = nullptr;
	//老鼠最后获取的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_MouseEndLocation;
	//是否只获取一次位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bGetOnce = true;
	//是否约束飞行物对行的重叠
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bConstraintLine = false;
	//当前的攻击线路(如果需要用到该条件)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Line = 0;
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ATK = 1.f;
	//第二攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float _SecondATK = 0.f;
	//可击打类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ELineType> M_AttackType;
	//携带的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItem_Buff M_FItem_Buff;
public:
	//初始化获取一次老鼠的位置
	UFUNCTION(BlueprintCallable)
		FVector GetMouseBeginLocationOnce();
	//获取老鼠的位置
	UFUNCTION(BlueprintCallable)
		FVector GetMouseActorLocation();
	//击中是否成功（bool 设置行约束->只能攻击本行单位   int32 是约束行）
	UFUNCTION(BlueprintCallable)
		bool Hit(const ELineType& _EMouseType, bool _bConstraint_Line = false, int32 _Constraint_Line = 0);
	//创建一个爆炸效果
	UFUNCTION(BlueprintCallable)
		void CreateExploed(float _LifeTime, float _ATK, FVector _BoxRange);
	//创建一个静态物品->作用可以进行二次伤害（范围伤害等）
	UFUNCTION(BlueprintCallable)
		void CreateStaticItem(FString _Path, FTransform _Trans, float _ATK);
	//获取攻击力
	UFUNCTION(BlueprintCallable)
		float GetATK();
	//获取第二攻击力
	UFUNCTION(BlueprintCallable)
		float GetSecondATK();

	//添加Buff
	//UFUNCTION(BlueprintCallable)
	//	void AddBuff();
};
