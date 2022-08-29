// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceManagerComponent.generated.h"

class AFlameActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UResourceManagerComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	//�Զ����ɻ���ļ�ʱ��
	FTimerHandle M_TimerHandleAutoSpawnFlame;
public:

	/*---------------------------------����---------------------------------*/
	//����Ĭ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_FlameNum = 500;
	/*---------------------------------����---------------------------------*/


	//�Ƿ��ǰ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDayTime = true;
public:
	// Sets default values for this component's properties
	UResourceManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/*---------------------------------����---------------------------------*/
	//���������Զ�����
	void EnableAutoFlameSpawn(const float& _SpawnTime);
	//��ӻ���
	UFUNCTION(BlueprintCallable)
		void AddFlameNum(const int32& _Value);
	//���ٻ���
	UFUNCTION(BlueprintCallable)
		void SubtractFlameNum(const int32& _Value);
	//��ȡ����
	UFUNCTION(BlueprintCallable)
		const int32& GetCurrentFlameNum();
	/*---------------------------------����---------------------------------*/

	/*---------------------------------��---------------------------------*/

	//��ȡ�ǰ��컹��ҹ��
	UFUNCTION(BlueprintCallable)
		bool GetDay();
	/*---------------------------------��---------------------------------*/

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//��Ϸ��ʼ
	UFUNCTION(BlueprintCallable)
		void GameStart();

	//��Ϸ����
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
