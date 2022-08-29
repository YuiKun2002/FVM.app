// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetBase.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/GameConfigManager.h"
#include <Components/Image.h>
#include "Engine/Texture2D.h"
#include "Game/UI/Tips/UI_Tip.h"

/*

1.这个游戏是哔哩哔哩赖小宇ly一人制作的单机版游戏-美食大战老鼠。

2.这个游戏是免费游戏，作为Up的一个童年游戏，还是花了一定时间来制作的，因为是一个人做没有更多时间打磨细节希望理解

3. QQ:1033082401 群1:[924537454]   群2[:978043033]
	群3:[642884960] 群4:[1082582998] 群5:[674942175]

4.最后请支持正版游戏：4399美食大战老鼠官网[https://my.4399.com/yxmsdzls/]
*/

bool UWidgetBase::Initialize()
{
	if (!Super::Initialize())
		return false;

	return true;
}

void UWidgetBase::PlayOperateAudio()
{
	UFVMGameInstance::PlayBGM_S_Static(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_Operating_BGM, "GameOperationAudio");
}

void UWidgetBase::SetButtonStyle(UButton* _ButtonComponent, FString _ButtonTexturePath)
{
	if (_ButtonComponent)
	{
		FButtonStyle _Style;
		FSlateBrush _Normal, _Hovered, _Pressed;

		_Hovered.TintColor = FSlateColor(FLinearColor(FVector4(.7f, .7f, .7f, 1.f)));
		_Pressed.TintColor = FSlateColor(FLinearColor(FVector4(.5f, .5f, .5f, 1.f)));

		_Normal.DrawAs = ESlateBrushDrawType::Image;
		_Hovered.DrawAs = ESlateBrushDrawType::Image;
		_Pressed.DrawAs = ESlateBrushDrawType::Image;

		_Normal.SetResourceObject(UWidgetBase::WidgetLoadTexture2D(_ButtonTexturePath));
		_Hovered.SetResourceObject(UWidgetBase::WidgetLoadTexture2D(_ButtonTexturePath));
		_Pressed.SetResourceObject(UWidgetBase::WidgetLoadTexture2D(_ButtonTexturePath));

		_Style.SetNormal(_Normal);
		_Style.SetHovered(_Hovered);
		_Style.SetPressed(_Pressed);

		_ButtonComponent->SetStyle(_Style);
	}
}

void UWidgetBase::SetImageBrush(UImage* _ImageComponent, FString _ImageTexturePath, FVector _Color, float Alph, bool _ResetSize, FVector _Scale)
{
	if (_ImageComponent)
	{
		if (_ImageTexturePath.Equals(""))
		{
			_ImageComponent->SetVisibility(ESlateVisibility::Hidden);
			return;
		}

		UTexture2D* LoadImg = UWidgetBase::WidgetLoadTexture2D(_ImageTexturePath);

		if (LoadImg)
		{
			_ImageComponent->Brush.SetResourceObject(LoadImg);
			_ImageComponent->Brush.DrawAs = ESlateBrushDrawType::Image;
			UWidgetBase::SetImageColor(_ImageComponent, _Color, Alph);
			if (_ResetSize) {
				_ImageComponent->Brush.ImageSize = FVector2D(LoadImg->GetSizeX() * _Scale.X, LoadImg->GetSizeY() * _Scale.Y);
			}
			_ImageComponent->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
			_ImageComponent->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidgetBase::SetImageColor(UImage* _ImageComponent, FVector _Color, float Alph)
{
	if (!_ImageComponent)
		return;

	_ImageComponent->Brush.TintColor = FSlateColor(FLinearColor(_Color.X, _Color.Y, _Color.Z, Alph));
}

UTexture2D* UWidgetBase::WidgetLoadTexture2D(const FString& _Path)
{
	return LoadObject<UTexture2D>(0, *_Path);
}

void UWidgetBase::CreateTipWidget(const FString& _Text, FVector _Color, float Alpha)
{
	UUI_Tip* Tip = CreateWidget<UUI_Tip>(UFVMGameInstance::GetFVMGameInstance(), LoadClass<UUI_Tip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/BPUI_Tip.BPUI_Tip_C'")));
	Tip->SetTipText(_Text);
	Tip->SetTextColor(_Color, Alpha);
	Tip->AddToViewport();
}

void UWidgetBase::SetWidgetSacle(UWidget* _Widget, const FVector2D& _Sacle)
{
	if (_Widget)
	{
		FWidgetTransform _WidgetTrans;
		_WidgetTrans = _Widget->RenderTransform;
		_WidgetTrans.Scale = _Sacle;
		_Widget->SetRenderTransform(_WidgetTrans);
	}
}

UTexture2D* UWidgetBase::LoadTexture2D(const FString& _Path)
{
	return LoadObject<UTexture2D>(0, *_Path);
}
