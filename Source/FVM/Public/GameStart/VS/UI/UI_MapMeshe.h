// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Item/ItemStruct.h"
#include "UI_MapMeshe.generated.h"

class UImage;
class ACardActor;
class UCardManagerComponent;
class UUI_Card;

UCLASS()
class FVM_API UUI_MapMeshe : public UWidgetBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		UImage* M_Gird = nullptr;
public:
	//地图网格的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Tranform")
		FTransform M_MapMesheTransform;
	//格子类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Type")
		ELineType M_ELineType;
	//UIMapMeshe线路
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Line")
		FLine M_Line;
	//卡片信息集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapMeshe | Card")
		TMap<int32, ACardActor*> M_Card_Data;
private:
	//播放音效
	void PlayCard_PlayBGM();
	//放置卡片
	void PlayCard(UCardManagerComponent* _CardMComp, FItemCard& _CardData);
public:
	//获取当前网格所有的卡片数据
	const TMap<int32, ACardActor*>& GetCardDatas();
public:

	virtual bool Initialize() override;

	//设置颜色
	UFUNCTION(BlueprintCallable)
		void SetColor(FVector _Vec, float _Alpha);

	//销毁卡片
	bool EradicateCard(const int32& _CardLayer);

	UFUNCTION(BlueprintCallable)
		bool IdAdd(const int32 _ID, ACardActor* _Card);

	//卡片ID测试
	UFUNCTION(BlueprintCallable)
		bool TestID(const int32 _ID);

	//销毁卡片
	UFUNCTION(BlueprintCallable)
		void EradicateCard();

	//创建卡片实例
	UFUNCTION(BlueprintCallable)
		void CreateNewCard(UCardManagerComponent* _CardMComp);

	//获取当前网格的最顶部网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格位置
	UFUNCTION(BlueprintCallable)
		FVector GetMapMeshCurrentRight(int32 Row);
};
