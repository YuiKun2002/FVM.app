// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/Flipbook/FlipbookActorBase.h"
#include <Paper2D/Classes/PaperSprite.h>
#include <Paper2D/Classes/PaperFlipbook.h>
#include <Paper2D/Classes/PaperFlipbookComponent.h>

bool AFlipbookActorBase::AnimationIsEnd()
{
	//if (this->GetRenderComponent()->GetPlaybackPosition() >= this->GetRenderComponent()->GetFlipbook()->GetTotalDuration())
	//	return true;

	if (this->GetRenderComponent()->GetPlaybackPositionInFrames() + 1 == this->GetRenderComponent()->GetFlipbook()->GetNumKeyFrames())
	{
		//this->GetRenderComponent()->PlayFromStart();

		return true;
	}

	//	UE_LOG(LogTemp, Warning, TEXT("%.3f , %.3f"), this->GetRenderComponent()->GetPlaybackPosition(), this->GetRenderComponent()->GetFlipbook()->GetTotalDuration());

	return false;
}

bool AFlipbookActorBase::PlayAnimationSequence(const TArray<FString>& Anim_Flipbook_Path, bool PassCurrent, bool IsReset)
{

	if (IsReset)
		this->SequenceIndex = 0;

	//如果序列的索引大于了数组的最大个数(所有动画播放完毕之后返回true)
	if (this->SequenceIndex >= Anim_Flipbook_Path.Num())
		return this->ResetSequence();

	//开头
	if (PassCurrent && this->SequenceIndex == 0)
	{
		this->GetRenderComponent()->SetFlipbook(LoadObject<UPaperFlipbook>(0, *Anim_Flipbook_Path[this->SequenceIndex]));
		this->SequenceIndex++;
	}

	//开头之后中间的动画
	if (this->AnimationIsEnd())
	{
		this->GetRenderComponent()->SetFlipbook(LoadObject<UPaperFlipbook>(0, *Anim_Flipbook_Path[this->SequenceIndex]));
		this->SequenceIndex++;
	}

	//UE_LOG(LogTemp, Error, TEXT("%d"), Anim_Flipbook_Path.Num());

	return false;
}

int32 AFlipbookActorBase::PlayAnimationPerSecond(int32 _FramePerSecond)
{

	this->GetRenderComponent()->GetFlipbook();
	return this->GetRenderComponent()->GetFlipbook()->GetFramesPerSecond();
}

void AFlipbookActorBase::SetPlayAnimationRate(float _Value)
{
	this->GetRenderComponent()->SetPlayRate(_Value);
}

void AFlipbookActorBase::SetFlipbookColor(FVector _Color, float _Alpha)
{
	this->GetRenderComponent()->SetSpriteColor(FLinearColor(_Color.X, _Color.Y, _Color.Z, _Alpha));
}

bool AFlipbookActorBase::PlayEndDestroy()
{
	if (this->AnimationIsEnd())
	{
		this->Destroy();
		return true;
	}
	return false;
}

bool AFlipbookActorBase::ResetSequence()
{
	this->SequenceIndex = 0;
	return true;
}


AFlipbookActorBase::AFlipbookActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlipbookActorBase::BeginPlay()
{
	Super::BeginPlay();

	this->AddActorLocalOffset(this->M_OffsetPosition);
}

void AFlipbookActorBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (this->IsPlayEndOnDestroy)
	{
		this->PlayEndDestroy();
	}
}

bool AFlipbookActorBase::IsPendingKill(AActor* _Actor)
{
	if (!_Actor)
		return true;

	return _Actor->IsPendingKillPending();
}
