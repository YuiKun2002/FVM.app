// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSystem/Tools/GameSystemFunction.h"

FDateTime UGameSystemFunction::GameSystem_Time;
FDateTime UGameSystemFunction::GameSystem_Time_Now;
FRandomStream UGameSystemFunction::Random_Now;
bool UGameSystemFunction::bEnable = false;

void UGameSystemFunction::UpdateTime()
{
	if (UGameSystemFunction::bEnable)
	{
		UGameSystemFunction::GameSystem_Time_Now = UGameSystemFunction::GameSystem_Time.Now();
		UGameSystemFunction::Random_Now.Initialize(UGameSystemFunction::GameSystem_Time_Now.GetYear() + UGameSystemFunction::GameSystem_Time_Now.GetMonth() + UGameSystemFunction::GameSystem_Time_Now.GetDay() + UGameSystemFunction::GameSystem_Time_Now.GetHour() + UGameSystemFunction::GameSystem_Time_Now.GetMinute() + UGameSystemFunction::GameSystem_Time_Now.GetSecond());
	}
}

void UGameSystemFunction::OpenUpdate()
{
	UGameSystemFunction::bEnable = true;
}

void UGameSystemFunction::CloseUpdate()
{
	UGameSystemFunction::bEnable = false;
}

void UGameSystemFunction::ClearTimeClip(FTimerManager& _TimeManager, FTimeClip& _Clip)
{
	_TimeManager.ClearTimer(_Clip.M_FTimerHandle);
	_Clip.M_Count = 0;
}

void UGameSystemFunction::SaveCurrentPlayerData()
{
	UFVMGameInstance::GetFVMGameInstance()->SavePlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
}

void UGameSystemFunction::LoadCurrentPlayerData()
{
	UFVMGameInstance::GetFVMGameInstance()->LoadPlayerStructConfig(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_CurrentPlayerName);
}

FMaterialBase* UGameSystemFunction::GetMaterialDataFromBag(TArray<FMaterialBase>& _Arrays, const FString& _ItemName)
{
	for (FMaterialBase& Items : _Arrays)
	{
		if (Items.ItemName.Equals(_ItemName))
			return &Items;
	}
	return nullptr;
}

TArray<FMaterialBase*> UGameSystemFunction::GetMaterialsArrayByType(TArray<FMaterialBase>& _ArraysSource, EMaterialType _Type)
{
	TArray<FMaterialBase*> TempArrays;
	for (FMaterialBase& Item : _ArraysSource)
	{
		if (Item.M_MaterialType == _Type)
			TempArrays.Emplace(&Item);
	}
	return TempArrays;
}

FString UGameSystemFunction::GetCardGradeImagePath(int32 _Grade)
{
	// Texture2D'/Game/Resource/Texture/CardGrade/1.1'

	if (_Grade == 0)
		return FString();

	FString _GradePath = "Texture2D'/Game/Resource/Texture/CardGrade/" + FString::FormatAsNumber(_Grade) + "." + FString::FormatAsNumber(_Grade) + "'";
	return _GradePath;
}

bool UGameSystemFunction::GetRange(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	int32 _Result = UGameSystemFunction::Random_Now.RandRange(0, _EndMaxRange);;

	if (_ValueMaxRange >= _Result)
		return true;

	return false;
}

int32 UGameSystemFunction::GetRange_Int(int32 _ValueMaxRange, int32 _EndMaxRange)
{
	return UGameSystemFunction::Random_Now.RandRange(0, _EndMaxRange);
}

void UGameSystemFunction::Log(const FString& _FuncName, const FString& _Content)
{
	UE_LOG(LogTemp, Error, TEXT("%s %s"), *_FuncName, *_Content);
}
