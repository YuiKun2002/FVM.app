// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/FlipbookActorBase.h"
#include "GameStart/VS/MapBaseType.h"
#include "MouseActor.generated.h"

class UBoxComponent;
class ACardActor;
class AFlyItemActor;

//老鼠移动方向
UENUM(BlueprintType)
enum class EMouseMoveDirection : uint8 {
	_ETop UMETA(DispalyNme = "Top"),
	_EBottom UMETA(DispalyNme = "Bottom"),
	_ELeft UMETA(DispalyNme = "Left"),
	_ERight  UMETA(DispalyNme = "Right"),
};

USTRUCT(BlueprintType)
struct FMouseProperty_HP {
	GENERATED_USTRUCT_BODY()
public:
	//总生命值
	UPROPERTY()
		float M_HP = 10.f;
	//老鼠当前生命值
	UPROPERTY()
		float M_CurrentHP = 0.f;

	//头上总生命值
	UPROPERTY()
		float M_TotalHeadHP = 0.f;
	//头上面的防御生命值
	UPROPERTY()
		float M_HeadHP = 0.f;

	//前防御总生命值
	UPROPERTY()
		float M_TotalFrontDefenceHP = 0.f;
	//前面的防御生命值
	UPROPERTY()
		float M_FrontDefenceHP = 0.f;

	//身体总生命值
	UPROPERTY()
		float M_TotalBodyHP = 10.f;
	//身体上的生命值
	UPROPERTY()
		float M_BodyHP = 10.f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_ATK {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的总攻击力
	UPROPERTY()
		float M_ATK = 0.f;
	//老鼠的当前攻击力
	UPROPERTY()
		float M_CurrentATK = 0.f;
	//是否在攻击
	UPROPERTY()
		bool IsAttack = false;
	//是否受到伤害
	UPROPERTY()
		bool IsHurt = false;
	//受到的伤害值
	UPROPERTY()
		float HurtValue = 0.f;
};

USTRUCT(BlueprintType)
struct FMouseProperty_State {
	GENERATED_USTRUCT_BODY()
public:
	//移动速度
	UPROPERTY()
		float M_MoveSpeed = 15.f;
	//当前移动速度
	UPROPERTY()
		float M_CurrentMoveSpeed = 15.f;
	//是否可以移动
	UPROPERTY()
		bool M_IsMove = true;
	//是否死亡
	UPROPERTY()
		bool IsDeath = false;
	//老鼠移动的方向
	UPROPERTY()
		EMouseMoveDirection  M_EMouseMoveDirection = EMouseMoveDirection::_ELeft;
};

//一些条件
USTRUCT(BlueprintType)
struct FMouseProperty_Condition {
	GENERATED_USTRUCT_BODY()
public:
	//老鼠的类型
	UPROPERTY()
		ELineType M_EMouseType;
	//当前所在的行
	UPROPERTY()
		FLine M_Line;
	//所在的回合
	UPROPERTY()
		int32 M_AttachRound = 0;
	//播放速度
	UPROPERTY()
		float M_PlayRate = 1.f;
	//当前播放速度
	UPROPERTY()
		float M_CurrentPlayRate = 1.f;
	//是否处于Buff
	UPROPERTY()
		bool M_bBuffState = false;
	//（凝固和冻结最后返回false 其他返回true->false表示在tick中不能进行其他行为）
	UPROPERTY()
		bool M_bBuffResult = true;
};

//代理结构
template<class Type>
struct FuncDelegate {
public:
	FuncDelegate() {}
	FuncDelegate(void(Type::* _Func) ()) :Func(_Func) {}
	inline void SetDelegate(void(Type::* _Func) ()) { this->Func = _Func; }
public:
	void(Type::* Func) () = nullptr;
};

class AMouseActor;

//老鼠接收的信息
USTRUCT(BlueprintType)
struct FMouseProperty_Buff {
	GENERATED_USTRUCT_BODY()
public:
	//条件
	UPROPERTY()
		bool M_bCondition = false;
	//持续时间
	UPROPERTY()
		float M_Time = 0.f;
	//当前时间(Tick)
	UPROPERTY()
		float M_CurrentTime = 0.f;
public:
	//代理函数
	FuncDelegate<AMouseActor> M_FuncBegin;
	FuncDelegate<AMouseActor> M_FuncEnd;
private:
	//被代理类
	AMouseActor* M_CurrentMouseActor = nullptr;
public:
	//设置代理类
	void SetClass(AMouseActor* _MouseActor) { this->M_CurrentMouseActor = _MouseActor; }
	AMouseActor* GetClass() { return this->M_CurrentMouseActor; }
	//设置代理函数
	void SetBeginFunc(void (AMouseActor::* FuncBegin)()) { this->M_FuncBegin.Func = FuncBegin; }
	void SetEndFunc(void (AMouseActor::* FuncEnd)()) { this->M_FuncEnd.Func = FuncEnd; }
public:
	void Clear() {
		//如果当前buff已经不存在了
		if (!this->M_bCondition)
			return;

		//调用结束函数
		if (this->M_CurrentMouseActor)
			(this->M_CurrentMouseActor->*M_FuncEnd.Func)();

		this->M_bCondition = false;
		this->M_Time = 0.f;
		this->M_CurrentTime = 0.f;
	}
};

//声明buff代理函数
void SetBuffDelegate(AMouseActor* _Actor, FMouseProperty_Buff& _buff, void (AMouseActor::* FuncBegin)(), void (AMouseActor::* FuncEnd)());

//老鼠接收的信息
USTRUCT(BlueprintType)
struct FMouseProperty_RecvImformation {
	GENERATED_USTRUCT_BODY()
public:
	//减速
	UPROPERTY()
		FMouseProperty_Buff M_SlowDown;
	//加速
	UPROPERTY()
		FMouseProperty_Buff M_Accelerate;;
	//冻结
	UPROPERTY()
		FMouseProperty_Buff M_Freeze;
	//凝固
	UPROPERTY()
		FMouseProperty_Buff M_Solidification;
	//灼烧
	UPROPERTY()
		FMouseProperty_Buff M_Burn;
};


UCLASS()
class FVM_API AMouseActor : public AFlipbookActorBase
{
	GENERATED_BODY()
private:
	//血量
	UPROPERTY()
		FMouseProperty_HP M_Proper_HP;
	//攻击
	UPROPERTY()
		FMouseProperty_ATK M_Proper_ATK;
	//状态
	UPROPERTY()
		FMouseProperty_State M_Proper_State;
	//条件
	UPROPERTY()
		FMouseProperty_Condition M_Proper_Condition;
	//接收的信息
	UPROPERTY()
		FMouseProperty_RecvImformation M_RecvInformation;
private:
	//当前攻击的卡片对象
	UPROPERTY(EditAnywhere)
		TArray<ACardActor*> M_CurrentCardActor;
	//正在攻击的卡片
	UPROPERTY(EditAnywhere)
		ACardActor* M_CurrentAttackCardActor = nullptr;
	//老鼠的位置
	UPROPERTY()
		USceneComponent* M_MousePosition = nullptr;
protected:
	//添加Buff
	void AddBuffFunc(const float _DeltaTime, FMouseProperty_Buff& _buff, int32& _BuffCountResult);
public:
	//老鼠加速buff处理函数
	void AccelerateFunc();
	//老鼠加速buff处理函数结束
	void AccelerateFuncEnd();
	//老鼠减速Buff处理
	void SlowDownFunc();
	//老鼠减速Buff处理结束
	void SlowDownFuncEnd();
	//老鼠凝固
	void SolidificationFunc();
	//老鼠凝固函数处理结束
	void SolidificationFuncEnd();
	//老鼠冻结
	void FreezeFunc();
	//老鼠冻结函数结束
	void FreezeFuncEnd();
	//老鼠灼烧
	void BurnFunc();
	//老鼠灼烧函数结束
	void BurnFuncEnd();
	//解析Buff信息
	void ParseBuff_Information(AFlyItemActor* _Item);
public:

	//当前老鼠被消灭
	UFUNCTION(BlueprintCallable)
		void MouseKill(bool _Destory = false);

	//进入卡片的重叠范围
	UFUNCTION(BlueprintCallable)
		void InCardRange(ACardActor* _ACardActor);

	//老鼠被攻击（bool 约束行,0表示攻击前方的防御 1表示攻击到头部 或者身体）
	UFUNCTION(BlueprintCallable)
		bool Hit(AActor* _Item, bool _Constraint_Line = false, uint8 _HitType = 0);

	//更新生命值
	UFUNCTION(BlueprintCallable)
		void UpdateHP(float _HurtValue, uint8 _HitType);
	//设置生命值
	UFUNCTION(BlueprintCallable)
		void SetHP(float _BodyHP, float _HeadHP, float _FrontDefenceHP);

	//获取头部生命值
	UFUNCTION(BlueprintCallable)
		float GetHeadHP();
	UFUNCTION(BlueprintCallable)
		float GetTotalHeadHP();

	//获取前防御生命值
	UFUNCTION(BlueprintCallable)
		float GetFrontDefenceHP();
	UFUNCTION(BlueprintCallable)
		float GetTotalFrontDefenceHP();

	// 获取身体生命值
	UFUNCTION(BlueprintCallable)
		float GetBodyHP();
	UFUNCTION(BlueprintCallable)
		float GetTotalBodyHP();

	//获取当前生命值
	UFUNCTION(BlueprintCallable)
		float GetCurrentHP();
	//获取总生命值
	UFUNCTION(BlueprintCallable)
		float GetTotalHP();

	//获取当前速度
	UFUNCTION(BlueprintCallable)
		float GetCurrentMoveSpeed();
	//设置当前速度
	UFUNCTION(BlueprintCallable)
		void SetCurrentMoveSpeed(const float _Value);
	//获取原本的速度
	UFUNCTION(BlueprintCallable)
		float GetSpeed();
	//设置原本的速度
	UFUNCTION(BlueprintCallable)
		void SetSpeed(const float _Value);

	//获取当前攻击力
	UFUNCTION(BlueprintCallable)
		float GetCurrentATK();
	//获取原本的攻击力
	UFUNCTION(BlueprintCallable)
		float GetATK();
	//设置老鼠攻击力
	UFUNCTION(BlueprintCallable)
		void SetMouseATK(float _Value);

	//获取是否受到伤害
	UFUNCTION(BlueprintCallable)
		bool GetbIsHurt();
	//设置是否受到伤害
	UFUNCTION(BlueprintCallable)
		void SetbIsHurt(bool _Value);

	//获取受到的伤害值
	UFUNCTION(BlueprintCallable)
		float GetHurtValue();
	//设置受到的伤害值
	UFUNCTION(BlueprintCallable)
		void SetHurtValue(float _Value);

	//获取是否在攻击
	UFUNCTION(BlueprintCallable)
		bool GetbIsAttack();
	//设置是否在攻击
	UFUNCTION(BlueprintCallable)
		void SetbIsAttack(bool _Value);

	//开始移动
	UFUNCTION(BlueprintCallable)
		bool MoveStart();
	//移动停止
	UFUNCTION(BlueprintCallable)
		bool MoveStop();
	//获取是否可以移动
	UFUNCTION(BlueprintCallable)
		bool GetbIsMove();

	//设置老鼠线路类型
	UFUNCTION(BlueprintCallable)
		void SetMouseLineType(ELineType _Type);
	//获取老鼠路线类型
	UFUNCTION(BlueprintCallable)
		ELineType GetMouseLineType();

	//获取老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
		FLine& GetMouseLine();
	//设置老鼠当前的格子位置
	UFUNCTION(BlueprintCallable)
		void SetMouseLine(FLine _Line);

	//获取老鼠的位置
	UFUNCTION(BlueprintCallable)
		USceneComponent* const GetMouseLocation();
	//设置老鼠的位置
	UFUNCTION(BlueprintCallable)
		void SetMouseLocation(USceneComponent* _Position);

	//获取老鼠是否死亡
	UFUNCTION(BlueprintCallable)
		bool GetMouseIsDeath();
	//设置老鼠是否死亡
	UFUNCTION(BlueprintCallable)
		void SetMouseDeath(bool _Value);

	//是否处于Buff状态
	UFUNCTION(BlueprintCallable)
		bool GetBufff();
	//最终buff的结果->false 表示不能进行任何行为  true表示可以进行其他行为
	UFUNCTION(BlueprintCallable)
		bool GetBuffResult();

	//获取老鼠所在的回合
	UFUNCTION(BlueprintCallable)
		int32 GetMouseRound();
	//设置老鼠的所在回合
	UFUNCTION(BlueprintCallable)
		void SetMouseRound(int32 _Value);

	//获取当前正在攻击的卡片数组
	UFUNCTION(BlueprintCallable)
		TArray<ACardActor*>& GetCurrentAttackCards();
	//获取当前正在攻击的卡片
	UFUNCTION(BlueprintCallable)
		ACardActor* GetCurrentAttackCard();
	//设置正在攻击的卡片
	UFUNCTION(BlueprintCallable)
		ACardActor* SetCurrentAttackCard(ACardActor* _Card);

	//获取老鼠移动方向
	UFUNCTION(BlueprintCallable)
		EMouseMoveDirection GetMouseMoveDirection();
	//设置老鼠移动方向
	UFUNCTION(BlueprintCallable)
		void SetMouseMoveDirection(EMouseMoveDirection _Direction);

	//更新攻击对象(true表示还有攻击对象)
	UFUNCTION(BlueprintCallable)
		bool UpdateAttackObject();

	//攻击防御卡
	UFUNCTION(BlueprintCallable)
		bool MouseAttackCard();

	//添加一个新的buff
	UFUNCTION(BlueprintCallable)
		void AddNewBuff(FMouseProperty_Buff& _Buff, float _Time);
	//移除一个buff
	UFUNCTION(BlueprintCallable)
		void RemoveBuff(FMouseProperty_Buff& _Buff);
public:
	//Test

	//凝固buff
	UFUNCTION(BlueprintCallable)
		void AddSolidificationBuff(float _Time);
	//灼烧buff
	UFUNCTION(BlueprintCallable)
		void AddBurnBuff(float _Time);
	//冻结buff
	UFUNCTION(BlueprintCallable)
		void AddFreezeBuff(float _Time);
	//加速buff
	UFUNCTION(BlueprintCallable)
		void AddAccelerateBuff(float _Time);
	//减速buff
	UFUNCTION(BlueprintCallable)
		void AddSlowDownBuff(float _Time);
public:
	//Tick 更新
	virtual void Tick(float DeltaTime) override;
	//初始化
	virtual void BeginPlay() override;

	AMouseActor();
};
