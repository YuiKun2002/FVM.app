// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/MapMeshe.h"
#include "GameStart/VS/UI/UI_MapMeshe.h"
#include "GameStart/VS/Components/MapMesheWidgetComponent.h"
#include <Components/WidgetComponent.h>
// Sets default values
AMapMeshe::AMapMeshe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建格子组件
	this->M_MapMesheWidgetComponent = this->CreateDefaultSubobject<UMapMesheWidgetComponent>("M_UMapMesheWidgetComponent");
	//设置根组件
	this->SetRootComponent(this->M_MapMesheWidgetComponent);
}

void AMapMeshe::SetLine(const int32& _Row, const int32& _Col, const ELineType _Type)
{
	if (this->M_MapMesheWidgetComponent)
	{
		UUI_MapMeshe* L_UUI_MapMeshe = Cast<UUI_MapMeshe>(this->M_MapMesheWidgetComponent->GetWidget());
		if (L_UUI_MapMeshe)
		{
			this->M_Line.SetLine(_Row, _Col);
			L_UUI_MapMeshe->M_Line.SetLine(_Row, _Col);
			L_UUI_MapMeshe->M_ELineType = _Type;
			this->M_ELineType = _Type;
		}
	}
}

void AMapMeshe::SetWidgetComponentTransform(const FTransform& _ActorTransform)
{
	if (this->M_MapMesheWidgetComponent)
	{
		UUI_MapMeshe* L_UUI_MapMeshe = Cast<UUI_MapMeshe>(this->M_MapMesheWidgetComponent->GetWidget());
		if (L_UUI_MapMeshe)
		{
			this->_Trans = _ActorTransform;
			L_UUI_MapMeshe->M_MapMesheTransform = this->_Trans;
		}
	}
}

FTransform AMapMeshe::GetWidgetComponentTransform()
{
	if (this->M_MapMesheWidgetComponent)
	{
		UUI_MapMeshe* L_UUI_MapMeshe = Cast<UUI_MapMeshe>(this->M_MapMesheWidgetComponent->GetWidget());
		if (L_UUI_MapMeshe)
		{
			return  L_UUI_MapMeshe->M_MapMesheTransform;
		}
	}

	return this->_Trans;
}

UUI_MapMeshe* AMapMeshe::GetUIMapMeshe()
{
	return  Cast<UUI_MapMeshe>(this->M_MapMesheWidgetComponent->GetWidget());;
}

// Called when the game starts or when spawned
void AMapMeshe::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMapMeshe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

