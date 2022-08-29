// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardCoolDownManager.generated.h"

UCLASS()
class FVM_API ACardCoolDownManager : public AActor
{
	GENERATED_BODY()
private:
	//��ȴʱ��
	UPROPERTY(EditAnywhere)
		float CoolDownTime = 4.f;
	//��ǰ��ȴʱ��
	UPROPERTY(EditAnywhere)
		float CurrentCoolDownTime = 0.f;
	//�Ƿ���ȴ���
	UPROPERTY(EditAnywhere)
		bool IsCoolDownFinish = false;
public:
	// Sets default values for this actor's properties
	ACardCoolDownManager();

	//��ȡ��ɵı���
	UFUNCTION(BlueprintCallable)
		float GetFinishRate();

	//ֱ�������ȴ
	UFUNCTION(BlueprintCallable)
		void SetCoolDownFinish(bool _finish = true);

	//�ж��Ƿ���ȴ���
	UFUNCTION(BlueprintCallable)
		bool GetIsCoolDownFinish();

	//������ȴ
	UFUNCTION(BlueprintCallable)
		void SetCoolDownEnable(bool _enable = true);

	//������ȴʱ��
	UFUNCTION(BlueprintCallable)
		void SetCoolDownTime(const float& _cooldown_time);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
