// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "FlipbookActorBase.generated.h"

class UPaperSprite;
class UPaperFlipbook;

UCLASS()
class FVM_API AFlipbookActorBase : public APaperFlipbookActor
{
	GENERATED_BODY()
public:
	//动画播放结束后销毁对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AFlipbookCondition)
		bool IsPlayEndOnDestroy = false;
	//序列播放索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AFlipbookCondition)
		int32 SequenceIndex = 0;
	//初始化完毕之后会自动加上这个偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AFlipbookLocationOffset)
		FVector M_OffsetPosition;
	//自定义Actor的拥有者
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AFlipbookCustomActorOwner)
		AActor* M_CustomActorOwner = nullptr;
private:
	bool _bPlayEnd = false;
public:
	AFlipbookActorBase();
	//开始
	virtual void BeginPlay() override;
	//帧
	virtual void Tick(float DeltaSeconds) override;
public:
	//判断该Actor是否等待被销毁
	UFUNCTION(BlueprintCallable)
		bool IsPendingKill(AActor* _Actor);
	//当前播放的动画是否播放完毕了
	UFUNCTION(BlueprintCallable)
		bool AnimationIsEnd();
	//播放动画组(所有动画播放完毕之后返回true) 需要Tick更新
	UFUNCTION(BlueprintCallable)
		bool PlayAnimationSequence(const TArray<FString>& Anim_Flipbook_Path, bool PassCurrent = false, bool IsReset = false);
	//设置关键帧数量->设置之前返回上一个设置的帧率
	UFUNCTION(BlueprintCallable)
		int32 PlayAnimationPerSecond(int32 _FramePerSecond);
	//设置播放速度
	UFUNCTION(BlueprintCallable)
		void SetPlayAnimationRate(float _Value);
	//设置翻书的颜色
	UFUNCTION(BlueprintCallable)
		void SetFlipbookColor(FVector _Color = FVector(1.f), float _Alpha = 1.f);
protected:
	//动画播放结束之后销毁对象
	bool PlayEndDestroy();
	//重置序列索引
	bool ResetSequence();
};
