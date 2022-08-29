// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStart/Flipbook/FlipbookActorBase.h"
#include "GameStart/VS/MapBaseType.h"
#include "GameSystem/Item/ItemStruct.h"
#include "GameStart/VS/Components/MouseManagerComponent.h"
#include "CardActor.generated.h"

class UMouseManagerComponent;
class AGameMapInstance;
class UUI_MapMeshe;
class AMouseActor;
class UUI_Card;

class UPaperFlipbookComponent;


//攻击类数据
USTRUCT(BlueprintType)
struct FCard_ATK {
	GENERATED_USTRUCT_BODY()
public:
	//攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK = 2.f;
	//第二攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SecondATK = 0.f;
	//攻击速度(延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 3.5f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
	//射线开始偏移(起点)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_TraceBeginOffset = FVector(-1.f, 35.f, -30.f);
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> M_LineTraceSettings;
};

//生命数据
USTRUCT(BlueprintType)
struct FCard_HP {
	GENERATED_USTRUCT_BODY()
public:
	//当前生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_Life = 10.f;
	//总生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_TotalLife = 10.f;
};

//生产类数据
USTRUCT(BlueprintType)
struct FCard_Spawn {
	GENERATED_USTRUCT_BODY()
public:
	//生产速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnSpeed = 0.f;
	//每生产一个的能量数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_SpawnCount = 0;
	//生产火苗的个数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_SpawnFlameCount = 0;
	//是否可以自动生长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SpawnCardIsAutoUp = false;
	//生长周期
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnCardUpTime = 0.f;
	//生长之后火苗提升值(倍率)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnCardFlameNumRate = 2.f;
};

//功能数据
USTRUCT(BlueprintType)
struct FCard_Function {
	GENERATED_USTRUCT_BODY()
public:
	//当前功能卡片数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItemCardFunction M_CardFunc_Data;
};

UCLASS()
class FVM_API ACardActor : public AFlipbookActorBase
{
	GENERATED_BODY()
private:
	//游戏地图实例
	UPROPERTY(EditAnywhere)
		AGameMapInstance* M_AGameMapInstance = nullptr;
	//卡片等级动画
	UPROPERTY(EditAnywhere)
		UPaperFlipbookComponent* M_CardGradeFlip = nullptr;
private:
	//颜色设置控制变量
	bool M_bColorSet = false;
public:
	//uiCard
	UUI_Card* M_UUI_Card = nullptr;
public:
	//攻击类
	UPROPERTY(EditAnywhere)
		FCard_ATK M_FCardActor_ATK;
	//生命类
	UPROPERTY(EditAnywhere)
		FCard_HP M_FCardActor_HP;
	//生产类
	UPROPERTY(EditAnywhere)
		FCard_Spawn M_FCardActor_Spawn;
	//功能类
	UPROPERTY(EditAnywhere)
		FCard_Function M_FCardActor_Function;

	//白天或者是夜晚(地图)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Day = true;
	//白天或者是夜晚(卡片)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardDay = true;

	//所有资源的集合
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ResourceRefPath)
		TArray<FString> M_ResourcePath;

	//当前卡片所在的UI网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUI_MapMeshe* M_UUI_MapMeshe = nullptr;

	//卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardName;

	//卡片优先级(一个格子只能存在不同优先级的卡片(只能存在一次))
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardID = 0;

	//当前卡片所在的路线
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLine M_Line;

	//适应类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType;

	//当前需要攻击的老鼠对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMouseActor* M_CurrentMouse = nullptr;
private:
	void SetCardActor(const FItemCard& _CardData);
public:
	//获取线路
	UFUNCTION(BlueprintCallable)
		FLine GetLine();

	//设置攻击类型的卡片数据
	UFUNCTION(BlueprintCallable)
		void SetCardData_ATK(const FItemCardATK& _ATK_Data);
	//设置防御类型的卡片数据
	UFUNCTION(BlueprintCallable)
		void SetCardData_Defence(const FItemCardDefence& _Defence_Data);
	//设置生产类型的卡片数据
	UFUNCTION(BlueprintCallable)
		void SetCardData_Spawn(const FItemCardSpawn& _Spwan_Data);
	//设置功能类型的卡片数据
	UFUNCTION(BlueprintCallable)
		void SetCardData_Function(const FItemCardFunction& _Function_Data);

	//获取生产速度
	UFUNCTION(BlueprintCallable)
		float GetCardSpawnSpeed();
	//获取生产的火苗值
	UFUNCTION(BlueprintCallable)
		int32 GetCardSpawnFlameNum();
	//获取火苗生产个数
	UFUNCTION(BlueprintCallable)
		int32 GetCardSpawnFlameCount();
	//获取生产是否可以自动生长
	UFUNCTION(BlueprintCallable)
		bool GetCardSpawnAutoWeekUp();
	//获取生产生长周期
	UFUNCTION(BlueprintCallable)
		float GetCardSpawnWeekUpTime();
	//获取火苗生产火苗提升倍率
	UFUNCTION(BlueprintCallable)
		float GetCardSpawnFlameRate();

	//获取功能类数据
	UFUNCTION(BlueprintCallable)
		FItemCardFunction GetCardFunctionData();

	//设置卡片等级
	UFUNCTION(BlueprintCallable)
		void SetCardGrade(const int32& _CardGrade);
	//设置类型和层
	UFUNCTION(BlueprintCallable)
		void SetCardType(const ELineType& _Type, const int32& _CardID);

	//卡片攻击力设置
	UFUNCTION(BlueprintCallable)
		void SetCardATK(const float& _BaseATK, const float& _ATKRate, const int32& _CardGrade);
	//获取卡片攻击力
	UFUNCTION(BlueprintCallable)
		float GetCardATK();

	//设置第二攻击力
	UFUNCTION(BlueprintCallable)
		void SetCardSecondATK(const float& _ATKRate);
	//获取第二攻击力
	UFUNCTION(BlueprintCallable)
		float GetCardSecondATK();
	//设置攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardAttackSpeed(float _Value);
	//获取攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardAttackSpeed();
	//设置首次攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardFirstAttackSpeed(float _Value);
	//获取首次攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardFirstAttackSpeed();
	//设置连续攻击速度
	UFUNCTION(BlueprintCallable)
		void SetCardAttackProjectionSpeed(float _Value);
	//获取连续攻击速度
	UFUNCTION(BlueprintCallable)
		float GetCardAttackProjectionSpeed();
	//设置攻击个数
	UFUNCTION(BlueprintCallable)
		void SetCardAttackItemCount(int32 _Value);
	//获取攻击个数
	UFUNCTION(BlueprintCallable)
		int32 GetCardAttackItemCount();
	//获取射线偏移
	UFUNCTION(BlueprintCallable)
		TArray<FLineTraceSetting> GetLineTraceSetting();


	//获取当前生命值
	UFUNCTION(BlueprintCallable)
		float GetCurrentHP();
	//获取总生命值
	UFUNCTION(BlueprintCallable)
		float GetTotalHP();

	//设置卡片的生命值
	UFUNCTION(BlueprintCallable)
		void SetCardHP(const float& _HP, float _HPUpRate);
	//设置卡片当前生命值
	UFUNCTION(BlueprintCallable)
		void SetCardCurrentHP(const float& _HP);

	//获取地图是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetMapDay();
	//获取卡片是白天还是夜晚
	UFUNCTION(BlueprintCallable)
		bool GetCardDay();
	//转换卡片适应的白天或者夜晚
	UFUNCTION(BlueprintCallable)
		void CardDayTransform(bool _Day);
	//获取地图状态是否游戏暂停
	UFUNCTION(BlueprintCallable)
		bool GetGameIsPause();

	//获取当前指向的老鼠
	UFUNCTION(BlueprintCallable)
		AMouseActor* GetCurrentMouse();
	//设置当前老鼠指向
	UFUNCTION(BlueprintCallable)
		void SetCurrentMouse(AMouseActor* _MouseActor);

	//获取卡片等级
	UFUNCTION(BlueprintCallable)
		int32 GetCardGrade(const FString& _Name);
	//获取老鼠线路通知
	UFUNCTION(BlueprintCallable)
		FMouseLineNotify& GetMouseLineNotify();
	//更新卡片状态（受到的攻击力和受到的回复）
	UFUNCTION(BlueprintCallable)
		bool UpdateCardState(const float& _ATK_Value, const float& _UpHP_Value = 0.f);
	//打印老鼠线路通知
	UFUNCTION(BlueprintCallable)
		void PrintMouseLineNotify();
	//移除当前卡片
	UFUNCTION(BlueprintCallable)
		void KillCard();

	//设置地图网格UI
	UFUNCTION(BlueprintCallable)
		void SetUIMapMesh(UUI_MapMeshe* _MapMesh);
	//获取地图网格UI
	UFUNCTION(BlueprintCallable)
		UUI_MapMeshe* GetUIMapMesh();

	//解析actor
	UFUNCTION(BlueprintCallable)
		void AnalysisActor(AActor* _Actor);
public:
	ACardActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
};
