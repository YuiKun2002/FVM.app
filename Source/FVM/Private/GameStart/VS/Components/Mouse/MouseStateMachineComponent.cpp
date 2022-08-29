// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/Mouse/MouseStateMachineComponent.h"
#include "GameStart/Flipbook/GameActor/MouseActor.h"

#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

#include "GameStart/Flipbook/GameActor/CardActor.h"

#include "GameStart/VS/MapMeshe.h"

// Sets default values for this component's properties
UMouseStateMachineComponent::UMouseStateMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

float UMouseStateMachineComponent::GetHPState(float _TotalHP, float _HP)
{
	if (_HP >= _TotalHP * 0.6f)
		return 0.6f;

	if (_HP >= _TotalHP * 0.3f)
		return 0.3f;

	if (_HP < _TotalHP * 0.3f && _HP > 0.f)
		return 0.1f;


	return 0.0f;
}

void UMouseStateMachineComponent::SetMouseType(bool _Ground, bool _OnWater, bool _Sky, bool _UnderWater)
{
	this->M_bGround = _Ground;
	this->M_bOnWater = _OnWater;
	this->M_bSky = _Sky;
	this->M_bUnderWater = _UnderWater;
}

bool  UMouseStateMachineComponent::ChangeLineType(ELineType _LineType)
{
	switch (_LineType)
	{
	case ELineType::OnGround:
		if (this->M_bGround)
		{
			this->M_Owner->SetMouseLineType(ELineType::OnGround);
			this->M_ELineType = ELineType::OnGround;
			return true;
		}break;
	case ELineType::Sky:
		if (this->M_bSky)
		{
			this->M_Owner->SetMouseLineType(ELineType::Sky);
			this->M_ELineType = ELineType::Sky;
			return true;
		}break;
	case ELineType::OnWater:
		if (this->M_bOnWater)
		{
			this->M_Owner->SetMouseLineType(ELineType::OnWater);
			this->M_ELineType = ELineType::OnWater;
			return true;
		}break;
	case ELineType::Underwater:
		if (this->M_bUnderWater)
		{
			this->M_Owner->SetMouseLineType(ELineType::Underwater);
			this->M_ELineType = ELineType::Underwater;
			return true;
		}break;
	case ELineType::DisEnable:
	{
		this->M_Owner->SetMouseLineType(ELineType::OnGround);
		this->M_ELineType = ELineType::OnGround;
		return true;
	}break;
	}
	return false;
}

bool UMouseStateMachineComponent::JudgeCurrentMeshLineType(AMapMeshe* _MapMesh)
{
	//同步老鼠的行列
	if (_MapMesh->M_Line.Row == this->M_Owner->GetMouseLine().Row)
		this->M_Owner->SetMouseLine(_MapMesh->M_Line);
	else
		return true; //当前网格和当前老鼠不同行

	//当前网格于老鼠同行
	return this->ChangeLineType(_MapMesh->M_ELineType);
}

void UMouseStateMachineComponent::SetAnimPath(const FString& _AnimPath)
{
	this->M_MouseAnimPath = _AnimPath;
}

void UMouseStateMachineComponent::PlayAnim(FString _Anim, bool IsPlayOnceToJump, FString _JumpAnim)
{
	FString _Anim_ = this->M_MouseAnimPath + _Anim + "." + _Anim + "'";
	this->M_bIsAnimCurrentPlayOnceToJump = IsPlayOnceToJump;
	this->M_JumpAnimName = _JumpAnim;
	this->M_CurrentPlayAnim = _Anim;
	this->M_Owner->GetRenderComponent()->SetFlipbook(LoadObject<UPaperFlipbook>(0, *_Anim_));
}


EMouseState UMouseStateMachineComponent::GetState(float _TotalHP, float _HP)
{
	//动画路径 和 动作
	if (this->M_Owner->GetCurrentHP() <= 0.f)
	{
		return EMouseState::D;
	}
	else
	{
		if (this->GetHPState(_TotalHP, _HP) == 0.6f)
		{
			return EMouseState::A;
		}
		else if (this->GetHPState(_TotalHP, _HP) == 0.3f)
		{
			return EMouseState::B;
		}
		else if (this->GetHPState(_TotalHP, _HP) > 0.f)
		{
			return EMouseState::C;
		}
		else {
			return EMouseState::D;
		}
	}
}



// Called when the game starts
void UMouseStateMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->M_Owner = Cast<AMouseActor>(this->GetOwner());
	if (!this->M_Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("[UMouseStateMachineComponent::BeginPlay]:老鼠对象获取失败"));
		this->SetComponentTickEnabled(false);
	}
}


// Called every frame
void UMouseStateMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (this->M_bIsAnimCurrentPlayOnceToJump)
	{
		if (this->M_Owner->AnimationIsEnd())
		{
			this->M_bIsAnimCurrentPlayOnceToJump = false;
			FString _Anim_ = this->M_MouseAnimPath + M_JumpAnimName + "." + M_JumpAnimName + "'";
			this->M_Owner->GetRenderComponent()->SetFlipbook(LoadObject<UPaperFlipbook>(0, *_Anim_));
		}
	}

	/*

	//如果老鼠在攻击
	if (this->M_Owner->IsAttack)
	{
		//当前动画播放完毕
		if (this->M_Owner->AnimationIsEnd())
		{
			//如果不能攻击则退出
			if (!this->M_bRunAttack)
				return;

			//重置攻击条件
			this->M_bRunAttack = false;

			ACardActor* CurrentCard = this->M_Owner->M_CurrentAttackCardActor;
			//如果卡片有效
			if (CurrentCard && !CurrentCard->IsPendingKillPending())
			{
				if (CurrentCard->UpdateCardState(this->M_Owner->M_ATK, 0.f))
				{
					//如果卡片死亡了关闭攻击模式
					this->M_Owner->IsAttack = false;
					return;
				}

				//如果卡片没有死亡则播放老鼠攻击动画
				return;
			}

			//如果卡片无效则取消老鼠攻击
			this->M_Owner->IsAttack = false;
		}
		else {
			//当前动画没有播放完毕则一直重置可攻击条件
			if (!this->M_bRunAttack)
				this->M_bRunAttack = true;
		}
	}
	else {

	}

	*/
}

