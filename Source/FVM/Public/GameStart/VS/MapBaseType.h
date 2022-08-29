// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapBaseType.generated.h"

//方向
UENUM(BlueprintType)
enum class EDirection : uint8 {
	_EUp  UMETA(DisplayName = "Up"),
	_EDown UMETA(DisplayName = "Down"),
	_ELeft UMETA(DisplayName = "Left"),
	_ERight UMETA(DisplayName = "Right"),
};

//当前属于的线路类型
UENUM(BlueprintType)
enum class ELineType : uint8 {
	//只允许陆地
	OnGround  UMETA(DisplayName = "OnGround"),
	//只允许适应空中的对象
	Sky UMETA(DisplayName = "Sky"),
	//只允许适应水上的对象
	OnWater UMETA(DisplayName = "OnWater"),
	//(只有适应水下的对象可以永久存在)
	Underwater UMETA(DisplayName = "UnderWater"),
	//允许任何位置
	All UMETA(DisplayName = "All"),
	//不允许被对象使用
	DisEnable UMETA(DisplayName = "DisEnable"),
};

//线路
USTRUCT(BlueprintType)
struct FLine
{
	GENERATED_USTRUCT_BODY()
public:
	FLine() {}
	FLine(const int32& _Row, const int32& _Col) :Row(_Row), Col(_Col) {}
	FLine(const int32& _Uniform) :Row(_Uniform), Col(_Uniform) {}

	void SetLine(const int32& _Row, const int32& _Col) {
		this->Row = _Row;
		this->Col = _Col;
	}

	bool operator==(const FLine& _Line)
	{
		if (this->Col == _Line.Col && this->Row == _Line.Row)
			return true;

		return false;
	}

	FLine& operator= (const FLine& _Line)
	{
		this->Col = _Line.Col;
		this->Row = _Line.Row;
		return *this;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Row = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Col = 0;
};

//地图的转换
USTRUCT(BlueprintType)
struct FMapTranform {
	GENERATED_USTRUCT_BODY()
public:
	//位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector M_Location = FVector(570.f, 0.f, 110.f);
	//旋转
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FRotator M_Rotation = FRotator(0.f, 90.f, 0.f);
	//缩放
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector M_Scale = FVector(1., 1.f, 1.f);
};

class UPaperSprite;
//地图精灵资源
USTRUCT(BlueprintType)
struct FMapSprite {
	GENERATED_USTRUCT_BODY()
public:
	//地图的背景
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UPaperSprite* M_MapBackgroundSprite = nullptr;
};


UCLASS()
class FVM_API UMapBaseType : public UObject
{
	GENERATED_BODY()
};
