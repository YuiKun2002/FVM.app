// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSystem/Item/ItemStruct.h"
#include "CardManagerComponent.generated.h"

class UUI_CardBar;
class ACardActor;
class ACardCoolDownManager;
class UUI_Card;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UCardManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	//卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FItemCard> M_CardData;
	//卡片列表
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> M_CardName;
	//UIcard数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UUI_Card*> M_UICard;
	//从卡片列表选择索引作为快捷创建
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<int32> M_SpecialCardListIndex;
	//卡片冷却
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<ACardCoolDownManager*> M_CardCoolDownManager;
	//当前选择的卡片
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString M_CurrentSelectCardName;
	//当前是否选择了铲子
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool M_CurrentSelectEradicate = false;
	//当前选中的卡片实例
	UPROPERTY(EditAnywhere)
		TSubclassOf<ACardActor> M_CurrentSelectCardInstance;
	//3DUI的Actor容器
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* M_3DUIActor = nullptr;
	//卡片选择界面
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_CardBar* M_UUI_CardBar = nullptr;
public:
	// Sets default values for this component's properties
	UCardManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	//获取当前选中的Actor对象
	UFUNCTION(BlueprintCallable)
		TSubclassOf<ACardActor>& GetCurrentSelectActor();
	//获取当前选中卡片的冷却
	UFUNCTION(BlueprintCallable)
		ACardCoolDownManager* GetCurrentCoolDownManager();
	//设置一张卡片的背景
	UFUNCTION(BlueprintCallable)
		void SetCard_Image(UUI_Card* _CardUI, const FString& _CardBg);
	//获取卡片的火苗数量
	UFUNCTION(BlueprintCallable)
		const int32 GetCardFlameNum(const FString& _CardActorName);
	//获取卡片数据
	UFUNCTION(BlueprintCallable)
		FItemCard& GetCardData(const FString& _CardActorName);
	//获取UICard
	UFUNCTION(BlueprintCallable)
		UUI_Card* const GetUICard(const FString& _CardActorName);
	//获取当前选择的卡片名称（通过UI选择卡片->选择卡片的名称）
	UFUNCTION(BlueprintCallable)
		const FString& GetCurrentSelectCardName();
	//选择一张卡片（通过UI选择卡片->选择）
	UFUNCTION(BlueprintCallable)
		bool SelectCurrentActor(const FString& _CardActorName);
	//取消当前选择的卡片（通过UI选择卡片->取消）
	UFUNCTION(BlueprintCallable)
		void CancelSelect();
	//选择铲子功能
	UFUNCTION(BlueprintCallable)
		void SelectEradicate();
	//取消铲子功能
	UFUNCTION(BlueprintCallable)
		void CancelEradicate();
	//添加卡片（到卡片列表存储起来）
	UFUNCTION(BlueprintCallable)
		void AddCard(const FItemCard& _CardData);
	//添加特别的卡片(必须从卡片列表中选取)
	UFUNCTION(BlueprintCallable)
		bool AddSpecialCard(const int32& _CardListIndex);
	//添加冷却时间（将卡片的冷却时间添加到列表存储）
	UFUNCTION(BlueprintCallable)
		void AddCoolDown(const float& _Time);
	//隐藏卡片栏
	UFUNCTION(BlueprintCallable)
		void HiddenCardBar();
	//显示卡片栏
	UFUNCTION(BlueprintCallable)
		void ShowCardBar();
	//获取指定卡片，是否冷却完成
	UFUNCTION(BlueprintCallable)
		bool GetCardCoolDownFinish(const int32& _Index);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//游戏开始
	UFUNCTION(BlueprintCallable)
		void GameStart();
	//游戏结束
	UFUNCTION(BlueprintCallable)
		void GameOver();
};
