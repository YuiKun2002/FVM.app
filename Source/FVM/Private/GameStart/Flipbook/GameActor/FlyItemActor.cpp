// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/GameActor/FlyItemActor.h"

FVector AFlyItemActor::GetMouseBeginLocationOnce()
{
	if (this->M_bGetOnce)
	{
		this->M_bGetOnce = false;
		this->M_MouseEndLocation = this->GetMouseActorLocation();
	}
	return this->M_MouseEndLocation;
}

FVector AFlyItemActor::GetMouseActorLocation()
{
	//老鼠对象失效,则让子弹全部失效

	if (!this->M_MouseActorLocation || this->M_MouseActorLocation->IsPendingKillPending())
	{
		return this->GetActorLocation();
	}

	//返回当前老鼠的位置
	const FVector& Mouse_Off = Cast<AMouseActor>(this->M_MouseActorLocation)->GetMouseLocation()->GetComponentLocation();

	this->M_MouseEndLocation = this->M_MouseActorLocation->GetActorLocation() + FVector(Mouse_Off.Y, Mouse_Off.X, Mouse_Off.Z);

	return this->M_MouseEndLocation;
}

bool AFlyItemActor::Hit(const ELineType& _EMouseType, bool _bConstraint_Line, int32 _Constraint_Line)
{
	this->M_bConstraintLine = _bConstraint_Line;

	for (const auto& Item : this->M_AttackType)
	{
		if (_bConstraint_Line)
		{
			if (Item == _EMouseType && this->M_Line == _Constraint_Line)
				return true;
		}
		else {
			if (Item == _EMouseType)
				return true;
		}
	}

	return false;
}

void AFlyItemActor::CreateExploed(float _LifeTime, float _ATK, FVector _BoxRange)
{

}

void AFlyItemActor::CreateStaticItem(FString _Path, FTransform _Trans, float _ATK)
{
	UClass* Obj = LoadClass<AFlyItemActor>(0, *_Path);

	if (!Obj)
	{
		UE_LOG(LogTemp, Error, TEXT("AFlyItemActor::CreateStaticItem:无效路径 %s"), *_Path);
		return;
	}

	FVector Location(_Trans.GetLocation());
	Location.X = 565.f;

	_Trans.SetLocation(Location);

	AFlyItemActor* Item = this->GetWorld()->SpawnActor<AFlyItemActor>(Obj, _Trans);
	Item->ATK = _ATK;
	Item->_SecondATK = _ATK;

	UE_LOG(LogTemp, Error, TEXT("%.2f"), _ATK);
}

float AFlyItemActor::GetATK()
{
	return this->ATK;
}

float AFlyItemActor::GetSecondATK()
{
	return this->_SecondATK;
}
