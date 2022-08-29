// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Data/CardData/CardDataStruct.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/FVMGameInstance.h"

#include "CardDataComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardDataComponent();

public:

	//获取攻击类型的卡片数据（卡片名称，是否从背包中获取）
	UFUNCTION(BlueprintCallable)
		FItemCardATK GetCardATKData(const FString& _CardName, bool _ToBag, int32 _CardGrade);
	//获取生产类型的卡片数据（卡片名称，是否从背包中获取）
	UFUNCTION(BlueprintCallable)
		FItemCardSpawn GetCardSpawnData(const FString& _CardName, bool _ToBag, int32 _CardGrade);
	//获取防御类型的卡片数据（卡片名称，是否从背包中获取）
	UFUNCTION(BlueprintCallable)
		FItemCardDefence GetCardDefenceData(const FString& _CardName, bool _ToBag, int32 _CardGrade);
	//获取功能类型的卡片数据（卡片名称，是否从背包中获取）
	UFUNCTION(BlueprintCallable)
		FItemCardFunction GetCardFunction(const FString& _CardName, bool _ToBag, int32 _CardGrade);

	template<class CardType = FItemCard, class CardStruct = FTableRowBase>
	CardType GetCardData(const FString& _CardName, ECardType _ECardType, bool _ToBag, int32 _CardGrade)
	{
		//从背包中寻找
		if (_ToBag)
		{
			for (FItemCard& Datas : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card)
			{
				if (Datas.ItemName.Equals(_CardName) && Datas.M_CardGrade == _CardGrade)
				{
					CardType Data = *(CardType*)(&Datas);
					return Data;
				}
			}

			return CardType();
		}

		//卡片数据
		CardType CardData;

		//获取类型的卡片
		UPlayerStructManager::GetCardData<CardType, CardStruct>(CardData, _CardName, _ECardType, _CardGrade);

		return CardData;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
