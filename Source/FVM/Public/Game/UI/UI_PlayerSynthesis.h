// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "GameSystem/Tools/GameSystemFunction.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/Item/ItemStruct.h"
#include <Components/UniformGridPanel.h>
#include "UI_PlayerSynthesis.generated.h"

class UUI_PlayerBagCardGrid;
class UUI_PlayerBagMaterialGrid;
class UUniformGridPanel;
class UButton;
class UCanvasPanel;
class UImage;

UCLASS()
class FVM_API UUI_PlayerSynthesis : public UWidgetBase
{
	GENERATED_BODY()
private:
	//时间分段加载结构
	FTimeClip M_FTimeClip_0;
	FTimeClip M_FTimeClip_1;
	FTimeClip M_FTimeClip_2;

	/*------------------------用于加载分段区分加载内容的成员属性(卡片加载->是否需要绑定函数加载卡片等级限制)------------------------*/
	int32 M_CardGrade = -1;
	bool M_IsBindFunction = false;
	FName M_BindFunctionName;

	//合成配方数据
	TArray<FMaterialBase*> M_Materials_CardMake_Arrays_Data;


	/*------------------------（LoadMaterials_Type函数专属）用于存储指定的材料数据的数组->比如:获取香料材料的数据四叶草的材料数据(指针指向背包中存储的数据)------------------------*/
	TArray<FMaterialBase*> M_Materials_Arrays_Data;
	bool M_bBindFunction_Materials = false;
	FName M_BindFunctionName_Materials;
	int32 M_LoadGridCount = 5;

	//强卡概率基数  =  基数 /  (主卡+副卡)

	//同星
	TArray<int32> M_UpGradeRate_0_Arrays = { 0,200,360,480,560,400,420,420,400,360,300,220,192,208,224,240,0 };
	//低一星
	TArray<int32> M_UpGradeRate_1_Arrays = { 100,300,440,420,423,429,403,390,374,285,273,376,250,270,361,248 };
	//低二星
	TArray<int32> M_UpGradeRate_2_Arrays = { 120,168,144,160,130,120,84,84,36,40,22,24,26,28 };

private:
	//制作卡片
	UPROPERTY()
		UButton* M_MakeCard_Butt = nullptr;
	//强化
	UPROPERTY()
		UButton* M_UpCard_Butt = nullptr;
	//转职
	UPROPERTY()
		UButton* M_ChangeCard_Butt = nullptr;
	//制作界面 
	UPROPERTY()
		UCanvasPanel* M_MakeCardP = nullptr;
	//强化界面
	UPROPERTY()
		UCanvasPanel* M_UpCardP = nullptr;
	//转职界面
	UPROPERTY()
		UCanvasPanel* M_ChangeCardP = nullptr;
	//材料显示界面
	UPROPERTY()
		UUniformGridPanel* M_Materials_Box = nullptr;
	//卡片显示界面
	UPROPERTY()
		UUniformGridPanel* M_Cards_Box = nullptr;
	//卡片显示界面2
	UPROPERTY()
		UUniformGridPanel* M_Cards_2_Box = nullptr;
	//材料显示界面2
	UUniformGridPanel* M_Materials_2_Box = nullptr;
public:
	/*----------------------------------------卡片制作----------------------------------------*/

	//制作卡片制作按钮-开始制作
	UPROPERTY()
		UButton* M_MakeCard_Run_Butt = nullptr;
	//香料按钮
	UPROPERTY()
		UButton* M_Spices_Butt = nullptr;
	//配方按钮
	UPROPERTY()
		UButton* M_SynthesisBlueprint_Butt = nullptr;

	//材料图片
	UPROPERTY()
		UImage* M_Material_0_Image = nullptr;
	UPROPERTY()
		UImage* M_Material_1_Image = nullptr;
	UPROPERTY()
		UImage* M_Material_2_Image = nullptr;

	//材料数组数据
	TArray<FMaterialBase*> M_MaterialsData;
	//配方数据
	FMaterialBase* M_MaterialBlueprintData = nullptr;
	//卡片配方名称
	FString M_MaterialBlueprintName;
	//制作卡片的目标名称
	FString M_MaterialBlueprintTargetName;
	//选择的香料名称
	FString M_SelectSpicesName;
	//香料的格子
	UUI_PlayerBagMaterialGrid* M_MakeCard_SpicesGrid = nullptr;

	/*----------------------------------------卡片制作----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	//四叶草按钮
	UPROPERTY()
		UButton* M_Clover_Butt = nullptr;
	//主卡
	UPROPERTY()
		UButton* M_UpGradeCard_Main_Butt = nullptr;
	//副卡1
	UPROPERTY()
		UButton* M_UpGradeCard_0_Butt = nullptr;
	//副卡2
	UPROPERTY()
		UButton* M_UpGradeCard_1_Butt = nullptr;
	//副卡3
	UPROPERTY()
		UButton* M_UpGradeCard_2_Butt = nullptr;
	//主卡等级
	UPROPERTY()
		UImage* M_UpGradeCard_Main_Grade = nullptr;
	//副卡0等级
	UPROPERTY()
		UImage* M_UpGradeCard_0_Grade = nullptr;
	//副卡1等级
	UPROPERTY()
		UImage* M_UpGradeCard_1_Grade = nullptr;
	//副卡2等级
	UPROPERTY()
		UImage* M_UpGradeCard_2_Grade = nullptr;
	//强化按钮
	UPROPERTY()
		UButton* M_UpGradeCard_Butt = nullptr;
	//卡片状态
	UPROPERTY()
		TArray<bool> M_UpGradeCardState = { false,false,false,false };
	//卡片格子
	UPROPERTY()
		TArray<UUI_PlayerBagCardGrid*> M_UpGradeCardGridUI = { nullptr,nullptr ,nullptr ,nullptr };
	//强化卡槽->在初始化里面添加
	UPROPERTY()
		TArray<UButton*> M_UpGradeSlot;
	//卡片强化文本显示
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_UpGradeCardRateText;
	//四叶草的格子
	UUI_PlayerBagMaterialGrid* M_MakeCard_CloverGrid = nullptr;
	/*----------------------------------------卡片强化----------------------------------------*/

	/*----------------------------------------卡片转职----------------------------------------*/

	//转职材料图片A
	UPROPERTY()
		UImage* M_ChangeJob_0_Image = nullptr;
	//转职材料图片B
	UPROPERTY()
		UImage* M_ChangeJob_1_Image = nullptr;
	//转职材料图片C
	UPROPERTY()
		UImage* M_ChangeJob_2_Image = nullptr;
	//转职材料图片C界面
	UPROPERTY()
		UCanvasPanel* M_ChangeJob_2_P = nullptr;
	//主卡按钮
	UPROPERTY()
		UButton* M_ChangeJobMainCard_Butt = nullptr;
	//转职按钮
	UPROPERTY()
		UButton* M_ChangeJob_Butt = nullptr;

	//转职卡片的目标名称
	FString M_CardChangeJobTargetName;
	//转职材料在背包的索引
	TArray<int32> M_CardChangeJobMaterialsIndex;
	//转职材料状态(拥有则true，未拥有则false)
	TArray<bool> M_CardChangeJobMaterialsArrays;
	//转职卡片的格子UI
	UUI_PlayerBagCardGrid* M_CardChangeJobGrid = nullptr;
	/*----------------------------------------卡片转职----------------------------------------*/



public:
	//初始化
	virtual bool Initialize() override;
	//显示界面
	void ShowPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index);
	//加载材料细节
	void LoadMaterialData(FMaterialBase* _Item, int32 _Index, int32 _Col, bool _bSelect, const FString& _SelectMaterialsName);
	//加载卡片细节
	void LoadCardData(FItemCard& _Item, int32 _Grade_, int32 _Index, int32 _Col, bool _IsBind, const FName _BindName, UUniformGridPanel* _ShowRange);
	//加载材料2细节
	void LoadMaterial_2_Data(FMaterialBase* _Item, int32 _Index, int32 _Col, bool _bBind, FName _BindFunctionName);
public:
	//显示制作卡片
	UFUNCTION(BlueprintCallable)
		void ShowMakeCard();
	//关闭界面
	UFUNCTION(BlueprintCallable)
		void ClearPanel();
	//显示强化卡片
	UFUNCTION()
		void ShowUpCard();
	//显示卡片转职
	UFUNCTION()
		void ShowChangeCard();
	//加载材料
	UFUNCTION(BlueprintCallable)
		void LoadMaterials();
	//加载材料区域2-香料
	UFUNCTION(BlueprintCallable)
		void LoadMaterialsSpices();
	//加载材料区域2-四叶草
	UFUNCTION(BlueprintCallable)
		void LoadMaterialsClover();
	//加载材料区域2-转职卡片
	UFUNCTION(BlueprintCallable)
		void LoadMaterialsCardChangeJobs();
	//加载卡片
	UFUNCTION(BlueprintCallable)
		void LoadCards();
	//加载卡片2号区域
	UFUNCTION(BlueprintCallable)
		void LoadCards_2(int32 _CardGrade = -1, bool _bIsBind = false, FName _BindFuncName = "");
	/*----------------------------------------卡片制作----------------------------------------*/
	UFUNCTION()
		void CancelSelectMakeCard();
	//取消香料的选择
	UFUNCTION()
		void CancelSelectSpices();
	//完全移除香料
	UFUNCTION()
		void CancelSpicesDestroy();
	UFUNCTION()
		void MakeCard();

	/*----------------------------------------卡片制作----------------------------------------*/

	/*----------------------------------------卡片强化----------------------------------------*/
	//取消选择的卡片->取消强化
	UFUNCTION(BlueprintCallable)
		void CancelSelectCard(int32 _Cancel_Index);
	//取消选择四叶草
	UFUNCTION(BlueprintCallable)
		void CancelSelectClover();
	//卡片强化升级
	UFUNCTION()
		void CardUpGrade();
	//获取概率最大数
	UFUNCTION()
		float GetUpGradeRate();
	/*----------------------------------------卡片强化----------------------------------------*/



	/*----------------------------------------卡片转职----------------------------------------*/
	//取消转职选择
	UFUNCTION(BlueprintCallable)
		void CancelSelectCardChangeJobs();
	//转职
	UFUNCTION()
		void CardChangeJobs();
	/*----------------------------------------卡片转职----------------------------------------*/



	//加载 香料  四叶草 转职道具专属函数
	/*----------------------------------------指定材料加载----------------------------------------*/
	template <EMaterialType MaterialType, int32 _LoadGrid, bool _bIsBindFunc>
	void LoadMaterials_Type(FName _BindFuncName)
	{
		//赋值数据
		this->M_bBindFunction_Materials = _bIsBindFunc;
		this->M_BindFunctionName_Materials = _BindFuncName;
		this->M_LoadGridCount = _LoadGrid;

		//清空UI
		this->M_Materials_2_Box->ClearChildren();
		//清空指向数据
		this->M_Materials_Arrays_Data.Empty();

		//材料数据(获取指定的材料数据)
		TArray<FMaterialBase>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
		this->M_Materials_Arrays_Data = UGameSystemFunction::GetMaterialsArrayByType(_Items_, MaterialType);

		//清除定时器
		UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_2);

		this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_2.M_FTimerHandle, [&]() {
			//判断索引是否有效
			if (this->M_FTimeClip_2.M_Count < this->M_Materials_Arrays_Data.Num()) {
				this->LoadMaterial_2_Data(this->M_Materials_Arrays_Data[this->M_FTimeClip_2.M_Count], this->M_FTimeClip_2.M_Count, this->M_LoadGridCount, this->M_bBindFunction_Materials, this->M_BindFunctionName_Materials);
				this->M_FTimeClip_2.M_Count++;
			}
			else {
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_2);
			}
			}, 0.01f, true);
	}
	/*----------------------------------------指定材料加载----------------------------------------*/



};
