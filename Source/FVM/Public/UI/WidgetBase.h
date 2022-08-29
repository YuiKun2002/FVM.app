// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "WidgetBase.generated.h"

#define F __FUNCTION__

class UUI_SelectTip;
class UButton;
class UImage;

class UUI_Tip;

UCLASS()
class FVM_API UWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
public:
	//播放操作音效
	UFUNCTION(BlueprintCallable)
		void PlayOperateAudio();
public:
	//绑定按钮事件
	template <class Type>
	static void ButtonDelegateBind(UButton* _ButtonComponent, Type* _DelegateClass, FName _FuncName)
	{
		FScriptDelegate Func;
		Func.BindUFunction(_DelegateClass, _FuncName);
		_ButtonComponent->OnClicked.Add(Func);
	}
public:
	//设置按钮风格
	UFUNCTION(BlueprintCallable)
		static void SetButtonStyle(UButton* _ButtonComponent, FString _ButtonTexturePath);
	//设置图片纹理
	UFUNCTION(BlueprintCallable)
		static void SetImageBrush(UImage* _ImageComponent, FString _ImageTexturePath, FVector _Color = FVector(1.f), float Alph = 1.f, bool _ResetSize = false, FVector _Scale = FVector(1.f, 1.f, 0.f));
	//设置图片颜色
	UFUNCTION(BlueprintCallable)
		static void SetImageColor(UImage* _ImageComponent, FVector _Color = FVector(1.f), float Alph = 1.f);
	//通过Widget加载2d纹理
	UFUNCTION(BlueprintCallable)
		static UTexture2D* WidgetLoadTexture2D(const FString& _Path);
	//创建一个提示
	UFUNCTION(BlueprintCallable)
		static void CreateTipWidget(const FString& _Text, FVector _Color = FVector(0.f, 1.f, 1.f), float Alpha = 1.f);
public:
	//设置画布缩放
	UFUNCTION(BlueprintCallable)
		void SetWidgetSacle(UWidget* _Widget, const FVector2D& _Sacle);
	//加载2D纹理
	UFUNCTION(BlueprintCallable)
		UTexture2D* LoadTexture2D(const FString& _Path);
public:
	template <class _Cast, class T = UWidgetBase>
	_Cast* GetWidgetComponent(T* _Widget, const FString& _ComponentName, const FString& _FuncName = "")
	{
		if (!_Widget)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:传入的_Widget是无效的"), *_FuncName);
			return nullptr;
		}
		//获取组件
		_Cast* WidgetComponent = Cast<_Cast>(_Widget->GetWidgetFromName(*_ComponentName));
		if (!WidgetComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s]:%s这个名称在传入的Widget中不存在!"), *_FuncName, *_ComponentName);
			return nullptr;
		}
		return WidgetComponent;
	}
};
