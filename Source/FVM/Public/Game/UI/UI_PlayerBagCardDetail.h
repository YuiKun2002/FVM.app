#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Item/ItemStruct.h"
#include "UI_PlayerBagCardDetail.generated.h"

/*
 * 卡片显示细节
 */

class UButton;
class UUI_PlayerBag;

UCLASS()
class FVM_API UUI_PlayerBagCardDetail : public UWidgetBase
{
	GENERATED_BODY()
public:
	//卡片价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardPrice;
	//冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardColdDown;
	//卡片星星等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardGrade;
	//卡片生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardHP;
	//是否可以转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_IsChange;
	//转职对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ChangeMaterialsName;
	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	//道具描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescrible;
	//道具描述2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescrible_2;
public:
	FItemCard* M_FItemCard = nullptr;
	//ui背包
	UUI_PlayerBag* M_PlayerUIBag = nullptr;
private:
	//删除卡片
	UPROPERTY()
		UButton* M_RemoveCard = nullptr;
public:
	virtual bool Initialize() override;
	//showTip
	UFUNCTION()
		void CreateTip();
	//删除卡片
	UFUNCTION()
		void RemoveCurrent();
};
