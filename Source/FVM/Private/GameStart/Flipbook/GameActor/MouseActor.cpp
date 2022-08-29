// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include "GameStart/Flipbook/GameActor/CardActor.h"
#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

#include "GameSystem/Tools/GameSystemFunction.h"

#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"


//设置Buff代理函数
void SetBuffDelegate(AMouseActor* _Actor, FMouseProperty_Buff& _buff, void (AMouseActor::* FuncBegin)(), void (AMouseActor::* FuncEnd)())
{
	_buff.SetClass(_Actor);
	_buff.M_FuncBegin.SetDelegate(FuncBegin);
	_buff.M_FuncEnd.SetDelegate(FuncEnd);
}

AMouseActor::AMouseActor()
{
	this->M_MousePosition = CreateDefaultSubobject<USceneComponent>("MousePosition");

	//初始化代理(加速buff代理)
	SetBuffDelegate(this, this->M_RecvInformation.M_Accelerate, &AMouseActor::AccelerateFunc, &AMouseActor::AccelerateFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Freeze, &AMouseActor::FreezeFunc, &AMouseActor::FreezeFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_SlowDown, &AMouseActor::SlowDownFunc, &AMouseActor::SlowDownFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Solidification, &AMouseActor::SolidificationFunc, &AMouseActor::SolidificationFuncEnd);
	SetBuffDelegate(this, this->M_RecvInformation.M_Burn, &AMouseActor::BurnFunc, &AMouseActor::BurnFuncEnd);
}

void AMouseActor::AddBuffFunc(const float _DeltaTime, FMouseProperty_Buff& _buff, int32& _BuffCountResult)
{
	//处理Buff
	if (_buff.M_bCondition)
	{
		//处理
		if (_buff.M_CurrentTime == 0.f && _buff.GetClass() && !_buff.GetClass()->IsPendingKillPending())
		{
			//调用Begin Delegate
			if (_buff.M_FuncBegin.Func)
				(_buff.GetClass()->*_buff.M_FuncBegin.Func)();
		}

		//计时
		_buff.M_CurrentTime += _DeltaTime;

		if (_buff.M_CurrentTime >= _buff.M_Time)
		{
			//调用End Delegate
			RemoveBuff(_buff);
		}

		//buff个数加1
		_BuffCountResult++;
	}
}

void AMouseActor::AccelerateFunc()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("加速"));

	//有减速buff
	if (this->M_RecvInformation.M_SlowDown.M_bCondition)
	{
		this->SetCurrentMoveSpeed(this->GetSpeed() * 2.f - this->GetSpeed() / 2.f);
		this->SetPlayAnimationRate(2.f - (1.f / 2.f));
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f));
	}
	else
	{
		this->SetCurrentMoveSpeed(this->GetCurrentMoveSpeed() * 2.f);
		this->SetPlayAnimationRate(2.f);
	}

	//如果出现凝固或者冻结那么动画静止
	if (this->M_RecvInformation.M_Solidification.M_bCondition || this->M_RecvInformation.M_Freeze.M_bCondition)
		this->SetPlayAnimationRate(0.f);
	else if (this->M_RecvInformation.M_SlowDown.M_bCondition)
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f));
	else
		this->SetFlipbookColor(FVector(1.f, 0.f, 0.f));
}

void AMouseActor::AccelerateFuncEnd()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("加速结束"));

	//有凝固buff 或者 有冻结buff
	if (this->M_RecvInformation.M_Solidification.M_bCondition || this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		this->SetPlayAnimationRate(0.f);
	}
	else if (this->M_RecvInformation.M_SlowDown.M_bCondition)//有减速buff
	{
		this->SetPlayAnimationRate(0.5f);
		this->SetCurrentMoveSpeed(this->GetSpeed() / 2.f);
	}
	else if (this->M_RecvInformation.M_Burn.M_bCondition)//有灼烧buff
	{
		this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f));
		this->SetCurrentMoveSpeed(this->GetSpeed());
	}
	else {
		this->SetCurrentMoveSpeed(this->GetSpeed());
		this->SetPlayAnimationRate(1.f);
		this->SetFlipbookColor();
	}
}

void AMouseActor::SlowDownFunc()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("减速"));

	//凝固和冻结
	if (this->M_RecvInformation.M_Solidification.M_bCondition || this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		this->SetPlayAnimationRate(0.f);
	}
	else {
		//没有凝固buff并且没有冻结才能使用
		this->SetPlayAnimationRate(0.5f);
		this->SetCurrentMoveSpeed(this->GetSpeed() / 2.f);
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f));
	}

	//如果有灼烧状态
	if (this->M_RecvInformation.M_Burn.M_bCondition)
	{
		RemoveBuff(this->M_RecvInformation.M_Burn);
		//结束减速状态
		RemoveBuff(this->M_RecvInformation.M_SlowDown);
	}

	//有加速buff
	if (this->M_RecvInformation.M_Accelerate.M_bCondition)
	{
		this->SetCurrentMoveSpeed(this->GetSpeed() * 2.f - this->GetSpeed() / 2.f);
		this->SetPlayAnimationRate(1.5f);
	}
}

void AMouseActor::SlowDownFuncEnd()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("减速结束"));

	//如果有凝固或者冻结buff
	if (this->M_RecvInformation.M_Solidification.M_bCondition || this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		this->SetPlayAnimationRate(0.f);
	}
	else if (this->M_RecvInformation.M_Accelerate.M_bCondition)//如果有加速buff
	{
		this->SetCurrentMoveSpeed(this->GetSpeed() * 2.f);
		this->SetPlayAnimationRate(2.f);
		this->SetFlipbookColor(FVector(1.f, 0.f, 0.f));
	}
	else if (this->M_RecvInformation.M_Burn.M_bCondition)//如果有灼烧buff
	{
		this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f));
		this->SetCurrentMoveSpeed(this->GetSpeed());
	}
	else {
		this->SetCurrentMoveSpeed(this->GetSpeed());
		this->SetPlayAnimationRate(1.f);
		this->SetFlipbookColor();
	}
}

void AMouseActor::SolidificationFunc()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("凝固或者冰冻"));

	this->SetPlayAnimationRate(0.f);
	this->MoveStop();
	this->M_Proper_Condition.M_bBuffResult = false;
}

void AMouseActor::SolidificationFuncEnd()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("凝固结束"));

	this->MoveStart();
	this->M_Proper_Condition.M_bBuffResult = true;

	//是否有冰冻buff
	if (this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		this->SetPlayAnimationRate(0.f);
		this->SetFlipbookColor(FVector(0.f, 0.f, 1.f));
		this->MoveStop();
		this->M_Proper_Condition.M_bBuffResult = false;
	}

	if (this->M_RecvInformation.M_SlowDown.M_bCondition)//有减速buff
	{
		this->SetCurrentMoveSpeed(this->M_RecvInformation.M_Accelerate.M_bCondition ? this->GetSpeed() * 2.f - this->GetSpeed() / 2.f : this->GetSpeed() / 2.f);
		this->SetPlayAnimationRate(this->M_RecvInformation.M_Accelerate.M_bCondition ? 1.5f : 0.5f);
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f));
	}
	else if (this->M_RecvInformation.M_Accelerate.M_bCondition)//有加速buff
	{
		this->SetCurrentMoveSpeed(this->GetCurrentMoveSpeed() * 2.f);
		this->SetPlayAnimationRate(2.0f);
		this->SetFlipbookColor(FVector(1.f, 0.f, 0.f));
	}

	if (this->M_RecvInformation.M_Burn.M_bCondition)//有灼烧buff
	{
		this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f));
	}

	//没有任何buff
	if (!this->M_RecvInformation.M_Burn.M_bCondition && !this->M_RecvInformation.M_Burn.M_bCondition && !this->M_RecvInformation.M_SlowDown.M_bCondition && !this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		this->SetPlayAnimationRate(1.f);
		this->SetCurrentMoveSpeed(this->GetSpeed());
		this->SetFlipbookColor();
	}
}

void AMouseActor::FreezeFunc()
{
	this->SolidificationFunc();
	this->SetFlipbookColor(FVector(0.f, 0.f, 1.f));

	//如果有灼烧状态
	if (this->M_RecvInformation.M_Burn.M_bCondition)
	{
		RemoveBuff(this->M_RecvInformation.M_Burn);
		//开启减速buff ->直接结束冻结时间  (冰冻和灼烧抵消)
		RemoveBuff(this->M_RecvInformation.M_Freeze);
		return;
	}
}

void AMouseActor::FreezeFuncEnd()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("冰冻结束"));

	//没有凝固buff
	if (!this->M_RecvInformation.M_Solidification.M_bCondition)
	{
		this->MoveStart();
		this->SetPlayAnimationRate(0.5f);
	}

	//抵消灼烧
	this->RemoveBuff(this->M_RecvInformation.M_Burn);

	//启动减速buff
	this->AddNewBuff(this->M_RecvInformation.M_SlowDown, 3.f);

	//判断是否死亡
	if (this->GetCurrentHP() <= 0.f)
		this->MoveStop();
}

void AMouseActor::BurnFunc()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("灼烧"));

	//先判断老鼠是否冻结
	if (this->M_RecvInformation.M_Freeze.M_bCondition)
	{
		RemoveBuff(this->M_RecvInformation.M_Freeze);
		this->SetFlipbookColor(FVector(0.f, 0.5f, 1.f));
		return;
	}

	//判断是否有减速buff
	if (this->M_RecvInformation.M_SlowDown.M_bCondition)
	{
		//消除减速
		RemoveBuff(this->M_RecvInformation.M_SlowDown);
		RemoveBuff(this->M_RecvInformation.M_Burn);
	}

	this->SetFlipbookColor(FVector(1.f, 0.2f, 0.f));
}

void AMouseActor::BurnFuncEnd()
{
	UGameSystemFunction::Log(__FUNCTION__, TEXT("灼烧结束"));

	//没有冻结buff和没有减速buff
	if (!this->M_RecvInformation.M_Freeze.M_bCondition && !this->M_RecvInformation.M_SlowDown.M_bCondition)
		this->SetFlipbookColor(FVector(1.f, 1.f, 1.f));
}

void AMouseActor::ParseBuff_Information(AFlyItemActor* _Item)
{
	auto AddBuff = [&](uint8 _Nums, float _Time)->void {
		switch (_Nums)
		{
		case 0:this->AddSlowDownBuff(_Time); break;
		case 1:this->AddFreezeBuff(_Time); break;
		case 2:this->AddSolidificationBuff(_Time); break;
		case 3:this->AddAccelerateBuff(_Time); break;
		default:this->AddBurnBuff(_Time); break;
		}
	};

	//是否携带buff条件
	if (_Item->M_FItem_Buff.M_bCondition)
	{
		for (auto Infor : _Item->M_FItem_Buff.M_Buffers)
		{
			AddBuff((uint8)(Infor.Key), Infor.Value);
		}
	}
}

void AMouseActor::MouseKill(bool _Destory)
{
	if (this->M_Proper_State.IsDeath)
		return;

	//停止移动
	this->MoveStop();
	//设置死亡
	this->M_Proper_State.IsDeath = true;
	//释放当前的所有攻击目标
	this->SetCurrentAttackCard(nullptr);
	//取消攻击
	this->SetbIsAttack(false);
	//设置攻击力为0
	this->SetMouseATK(0.f);

	UMouseManagerComponent* MouseManager = AGameMapInstance::GetGameMapInstance()->M_MouseManagerComponent;

	if (MouseManager->M_CurrentExistMouseCount > 0)
	{
		MouseManager->M_CurrentExistMouseCount--;

		//	UE_LOG(LogTemp, Warning, TEXT("当前老鼠:%d===存活:%d"), MouseManager->CurrentRoundSpawnMouseCount, MouseManager->M_CurrentExistMouseCount);
	}

	//如果为1 则销毁
	if (_Destory)
		this->Destroy();
}

void AMouseActor::InCardRange(ACardActor* _ACardActor)
{
	//进入范围
	this->M_CurrentCardActor.Emplace(_ACardActor);
}

bool AMouseActor::Hit(AActor* _Item, bool _Constraint_Line, uint8 _HitType)
{
	AFlyItemActor* Item = Cast<AFlyItemActor>(_Item);

	if (Item)
	{
		//UE_LOG(LogTemp, Error, TEXT("Hit! 解析飞行物的数据"));

		//约束老鼠
		if (_Constraint_Line && this->M_Proper_Condition.M_Line.Row == Item->M_Line)
		{
			this->M_Proper_ATK.IsHurt = true;
			this->UpdateHP(Item->ATK, _HitType);
			//解析Buff信息
			this->ParseBuff_Information(Item);
			return true;
		}
		else if (!_Constraint_Line) {
			//老鼠没有被约束但是飞行物被约束（递归一次，老鼠临时约束状态）
			if (Item->M_bConstraintLine)
				return Hit(_Item, true, 0);

			this->M_Proper_ATK.IsHurt = true;
			this->UpdateHP(Item->ATK, _HitType);
			//解析Buff信息
			this->ParseBuff_Information(Item);
			return true;
		}
	}
	return false;
}

void AMouseActor::UpdateHP(float _HurtValue, uint8 _HitType)
{
	float _Result = _HurtValue;

	//UE_LOG(LogTemp, Error, TEXT("%.2f"), _HurtValue);

	//前防御
	if (this->M_Proper_HP.M_FrontDefenceHP > 0.f)
	{
		this->M_Proper_HP.M_FrontDefenceHP -= _Result;

		if (this->M_Proper_HP.M_FrontDefenceHP <= 0.f)
		{
			_Result = this->M_Proper_HP.M_FrontDefenceHP + _Result;
			this->M_Proper_HP.M_FrontDefenceHP = 0.f;
		}
	}

	//头部防御
	if (this->M_Proper_HP.M_HeadHP > 0)
	{
		this->M_Proper_HP.M_HeadHP -= _Result;
		if (this->M_Proper_HP.M_HeadHP <= 0.f)
		{
			_Result = this->M_Proper_HP.M_HeadHP + _Result;
			this->M_Proper_HP.M_HeadHP = 0.f;
		}
	}

	if (_Result < 0.f)
		_Result = 0.f;

	//将身体血量扣除
	this->M_Proper_HP.M_BodyHP -= _Result;

	//血量设置
	if (this->M_Proper_HP.M_BodyHP <= 0.f)
		this->M_Proper_HP.M_BodyHP = 0.f;

	//更新生命值总和
	this->M_Proper_HP.M_CurrentHP = this->M_Proper_HP.M_BodyHP + this->M_Proper_HP.M_HeadHP + this->M_Proper_HP.M_FrontDefenceHP;
}

void AMouseActor::SetHP(float _BodyHP, float _HeadHP, float _FrontDefenceHP)
{
	this->M_Proper_HP.M_TotalBodyHP = _BodyHP;
	this->M_Proper_HP.M_TotalHeadHP = _HeadHP;
	this->M_Proper_HP.M_TotalFrontDefenceHP = _FrontDefenceHP;

	this->M_Proper_HP.M_BodyHP = _BodyHP;
	this->M_Proper_HP.M_HeadHP = _HeadHP;
	this->M_Proper_HP.M_FrontDefenceHP = _FrontDefenceHP;

	this->M_Proper_HP.M_HP = _BodyHP + _HeadHP + _FrontDefenceHP;
	this->M_Proper_HP.M_CurrentHP = this->M_Proper_HP.M_HP;
}

float AMouseActor::GetHeadHP()
{
	return this->M_Proper_HP.M_HeadHP;
}

float AMouseActor::GetTotalHeadHP()
{
	return this->M_Proper_HP.M_TotalHeadHP;
}

float AMouseActor::GetFrontDefenceHP()
{
	return this->M_Proper_HP.M_FrontDefenceHP;
}

float AMouseActor::GetTotalFrontDefenceHP()
{
	return this->M_Proper_HP.M_TotalFrontDefenceHP;
}

float AMouseActor::GetBodyHP()
{
	return this->M_Proper_HP.M_BodyHP;
}

float AMouseActor::GetTotalBodyHP()
{
	return this->M_Proper_HP.M_TotalBodyHP;
}

float AMouseActor::GetCurrentHP()
{
	return this->M_Proper_HP.M_CurrentHP;
}

float AMouseActor::GetTotalHP()
{
	return this->M_Proper_HP.M_HP;
}

float AMouseActor::GetCurrentMoveSpeed()
{
	return this->M_Proper_State.M_CurrentMoveSpeed;
}

void AMouseActor::SetCurrentMoveSpeed(const float _Value)
{
	this->M_Proper_State.M_CurrentMoveSpeed = _Value;
}

float AMouseActor::GetSpeed()
{
	return this->M_Proper_State.M_MoveSpeed;
}

void AMouseActor::SetSpeed(const float _Value)
{
	this->M_Proper_State.M_MoveSpeed = _Value;
	this->SetCurrentMoveSpeed(_Value);
}

float AMouseActor::GetCurrentATK()
{
	return this->M_Proper_ATK.M_CurrentATK;
}

float AMouseActor::GetATK()
{
	return this->M_Proper_ATK.M_ATK;
}

void AMouseActor::SetMouseATK(float _Value)
{
	this->M_Proper_ATK.M_CurrentATK = _Value;
	this->M_Proper_ATK.M_ATK = _Value;
}

bool AMouseActor::GetbIsHurt()
{
	return this->M_Proper_ATK.IsHurt;
}

void AMouseActor::SetbIsHurt(bool _Value)
{
	this->M_Proper_ATK.IsHurt = _Value;
}

float AMouseActor::GetHurtValue()
{
	return this->M_Proper_ATK.HurtValue;
}

void AMouseActor::SetHurtValue(float _Value)
{
	this->M_Proper_ATK.HurtValue = _Value;
}

bool AMouseActor::GetbIsAttack()
{
	return this->M_Proper_ATK.IsAttack;
}

void AMouseActor::SetbIsAttack(bool _Value)
{
	this->M_Proper_ATK.IsAttack = _Value;
}

bool AMouseActor::MoveStart()
{
	if (!this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = true;
		return true;
	}
	return false;
}

bool AMouseActor::MoveStop()
{
	if (this->M_Proper_State.M_IsMove)
	{
		this->M_Proper_State.M_IsMove = false;
		return true;
	}
	return false;
}

bool AMouseActor::GetbIsMove()
{
	return M_Proper_State.M_IsMove;
}

void AMouseActor::SetMouseLineType(ELineType _Type)
{
	this->M_Proper_Condition.M_EMouseType = _Type;
}

ELineType AMouseActor::GetMouseLineType()
{
	return this->M_Proper_Condition.M_EMouseType;
}

FLine& AMouseActor::GetMouseLine()
{
	return this->M_Proper_Condition.M_Line;
}

void AMouseActor::SetMouseLine(FLine _Line)
{
	this->M_Proper_Condition.M_Line = _Line;
}

USceneComponent* const AMouseActor::GetMouseLocation()
{
	return this->M_MousePosition;
}

void AMouseActor::SetMouseLocation(USceneComponent* _Position)
{
	this->M_MousePosition = _Position;
}

bool AMouseActor::GetMouseIsDeath()
{
	return this->M_Proper_State.IsDeath;
}

void AMouseActor::SetMouseDeath(bool _Value)
{
	this->M_Proper_State.IsDeath = _Value;
}

bool AMouseActor::GetBufff()
{
	return this->M_Proper_Condition.M_bBuffState;
}
bool AMouseActor::GetBuffResult()
{
	return this->M_Proper_Condition.M_bBuffResult;
}
int32 AMouseActor::GetMouseRound()
{
	return this->M_Proper_Condition.M_AttachRound;
}

void AMouseActor::SetMouseRound(int32 _Value)
{
	this->M_Proper_Condition.M_AttachRound = _Value;
}

TArray<ACardActor*>& AMouseActor::GetCurrentAttackCards()
{
	return this->M_CurrentCardActor;
}

ACardActor* AMouseActor::GetCurrentAttackCard()
{
	return this->M_CurrentAttackCardActor;
}

ACardActor* AMouseActor::SetCurrentAttackCard(ACardActor* _Card)
{
	this->M_CurrentAttackCardActor = _Card;
	return _Card;
}

EMouseMoveDirection AMouseActor::GetMouseMoveDirection()
{
	return this->M_Proper_State.M_EMouseMoveDirection;
}

void AMouseActor::SetMouseMoveDirection(EMouseMoveDirection _Direction)
{
	this->M_Proper_State.M_EMouseMoveDirection = _Direction;
}

bool AMouseActor::UpdateAttackObject()
{
	for (auto& Card : this->M_CurrentCardActor)
	{
		if (Card && !Card->IsPendingKillPending())
		{
			this->M_CurrentAttackCardActor = Card;
			return true;
		}
	}

	this->M_CurrentCardActor.Empty();
	return false;
}

bool AMouseActor::MouseAttackCard()
{
	//当前卡片无效
	if (!this->GetCurrentAttackCard())
		return false;

	//当前卡片有效
	if (!this->GetCurrentAttackCard()->IsPendingKillPending())
	{
		//当卡片死亡时
		if (this->GetCurrentAttackCard()->UpdateCardState(this->GetCurrentATK(), 0.f))
		{
			//销毁卡片
			this->GetCurrentAttackCard()->KillCard();

			//更新攻击目标
			this->UpdateAttackObject();

			//当前老鼠攻击的卡片生命为0则停止攻击

			//完全没有攻击目标之后
			if (this->M_CurrentCardActor.Num() == 0)
			{
				//结束攻击
				this->SetbIsAttack(false);
				return false;
			}
		}

		//卡片没有死亡 //还有目标继续攻击
		return true;
	}

	//更新攻击目标（没有攻击目标则不在攻击）
	if (!this->UpdateAttackObject())
	{
		//卡片被标记
		this->SetbIsAttack(false);
		return false;
	}

	return true;
}

void AMouseActor::AddNewBuff(FMouseProperty_Buff& _Buff, float _Time)
{
	this->M_Proper_Condition.M_bBuffState = true;
	_Buff.M_bCondition = true;
	_Buff.M_CurrentTime = 0.f;
	_Buff.M_Time = _Time;
}

void AMouseActor::RemoveBuff(FMouseProperty_Buff& _Buff)
{
	_Buff.Clear();
}

void AMouseActor::AddSolidificationBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Solidification, _Time);
}

void AMouseActor::AddBurnBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Burn, _Time);
}

void AMouseActor::AddFreezeBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Freeze, _Time);
}

void AMouseActor::AddAccelerateBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_Accelerate, _Time);
}

void AMouseActor::AddSlowDownBuff(float _Time)
{
	this->AddNewBuff(this->M_RecvInformation.M_SlowDown, _Time);
}

void AMouseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int32 M_BuffCount = 0;
	if (this->GetBufff())
	{
		//添加加速buff
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Solidification, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Freeze, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_SlowDown, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Accelerate, M_BuffCount);
		this->AddBuffFunc(DeltaTime, this->M_RecvInformation.M_Burn, M_BuffCount);

		//buff个数为0则不在处理
		if (!M_BuffCount)
			this->M_Proper_Condition.M_bBuffState = false;
	}

}

void AMouseActor::BeginPlay()
{
	Super::BeginPlay();

	//this->AddNewBuff(this->M_RecvInformation.M_Solidification, 2.f);
	//this->AddNewBuff(this->M_RecvInformation.M_Freeze, 1.f);
	//this->AddNewBuff(this->M_RecvInformation.M_Burn, 10.f);
	//this->AddNewBuff(this->M_RecvInformation.M_Accelerate, 10.f);
	//this->AddNewBuff(this->M_RecvInformation.M_SlowDown, 4.f);
}
