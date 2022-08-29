// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/UI/UI_Card.h"
#include "GameStart/VS/CardCoolDownManager.h"
#include "GameStart/VS/Components/CardManagerComponent.h"
#include "GameStart/VS/Components/ResourceManagerComponent.h"
#include "GameStart/VS/GameMapInstance.h"

#include <Components/Image.h>


bool UUI_Card::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UImage = this->GetWidgetComponent<UImage>(this, "Bg");

	this->M_UImage_Grade = this->GetWidgetComponent<UImage>(this, "Grade");

	return true;
}

bool UUI_Card::GetCoolDownFinish()
{
	if (this->M_CardIndex == -1)
		return false;

	if (AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->GetCardCoolDownFinish(this->M_CardIndex))
	{
		if (AGameMapInstance::GetGameMapInstance()->M_ResourceManagerComponent->M_FlameNum >= this->M_NeedFlame)
		{
			return true;
		}
	}

	return false;

}

float UUI_Card::GetCoolDownFinishRate()
{
	if (this->M_CardIndex == -1)
		return false;

	return AGameMapInstance::GetGameMapInstance()->M_CardManagerComponent->M_CardCoolDownManager[this->M_CardIndex]->GetFinishRate();
}

void UUI_Card::SetCardGrade(const int32& _Grade)
{
	if (_Grade > 0 && _Grade < 17)
	{
		FSlateBrush Image;
		Image.SetResourceObject(LoadTexture2D(FString("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FormatAsNumber(_Grade) + FString(".") + FString::FormatAsNumber(_Grade) + FString("'")));
		Image.DrawAs = ESlateBrushDrawType::Image;
		this->M_UImage_Grade->SetBrush(Image);
	}
	else {
		this->M_UImage_Grade->SetVisibility(ESlateVisibility::Hidden);
	}
}

UImage* UUI_Card::GetUImage()
{
	return this->M_UImage;
}

void UUI_Card::SelectColor()
{
	UWidgetBase::SetImageColor(this->GetUImage(), FVector(0.2f, 0.2f, 0.2f), 1.f);
}

void UUI_Card::CancelColor()
{
	UWidgetBase::SetImageColor(this->GetUImage(), FVector(1.f, 1.f, 1.f), 1.f);
}
