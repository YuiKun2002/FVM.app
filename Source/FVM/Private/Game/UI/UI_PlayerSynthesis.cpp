// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Button.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>


#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Game/UI/UI_PlayerBagCardGrid.h"

#include "GameSystem/FVMGameInstance.h"

bool UUI_PlayerSynthesis::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_MakeCard_Butt = this->GetWidgetComponent<UButton>(this, "MakeCard_butt");
	this->M_UpCard_Butt = this->GetWidgetComponent<UButton>(this, "UpCard_butt");
	this->M_ChangeCard_Butt = this->GetWidgetComponent<UButton>(this, "ChangeCard_butt");

	this->M_MakeCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Make_P");
	this->M_UpCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Up_P");
	this->M_ChangeCardP = this->GetWidgetComponent<UCanvasPanel>(this, "Change_P");

	this->M_Materials_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "Materials_Box");
	this->M_Cards_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box");
	this->M_Cards_2_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "Cards_Box_1");

	this->M_Materials_2_Box = this->GetWidgetComponent<UUniformGridPanel>(this, "UniformGridPanel_60");

	//按钮事件绑定
	this->M_MakeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowMakeCard);
	this->M_UpCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowUpCard);
	this->M_ChangeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::ShowChangeCard);

	/*----------------------------------------卡片制作----------------------------------------*/
	this->M_MakeCard_Run_Butt = this->GetWidgetComponent<UButton>(this, "Make_Butt");
	this->M_Spices_Butt = this->GetWidgetComponent<UButton>(this, "Material_3_Butt");
	this->M_SynthesisBlueprint_Butt = this->GetWidgetComponent<UButton>(this, "Main_Material_Butt");
	this->M_Material_0_Image = this->GetWidgetComponent<UImage>(this, "Material_0");
	this->M_Material_1_Image = this->GetWidgetComponent<UImage>(this, "Material_1");
	this->M_Material_2_Image = this->GetWidgetComponent<UImage>(this, "Material_2");

	//绑定取消选卡按钮
	this->M_SynthesisBlueprint_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSelectMakeCard);
	//绑定制作卡片按钮
	M_MakeCard_Run_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::MakeCard);
	//绑定香料的取消按钮
	this->M_Spices_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSpicesDestroy);
	/*----------------------------------------卡片制作----------------------------------------*/


	/*----------------------------------------卡片强化----------------------------------------*/

	this->M_UpGradeCard_Main_Butt = this->GetWidgetComponent<UButton>(this, "Button_98");
	this->M_UpGradeCard_0_Butt = this->GetWidgetComponent<UButton>(this, "Button");
	this->M_UpGradeCard_1_Butt = this->GetWidgetComponent<UButton>(this, "Button_1");
	this->M_UpGradeCard_2_Butt = this->GetWidgetComponent<UButton>(this, "Button_2");
	this->M_UpGradeCard_Butt = this->GetWidgetComponent<UButton>(this, "Upgrade");
	this->M_Clover_Butt = this->GetWidgetComponent<UButton>(this, "Button_3");
	//添加卡槽
	this->M_UpGradeSlot.Empty();
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_Main_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_0_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_1_Butt);
	this->M_UpGradeSlot.Emplace(this->M_UpGradeCard_2_Butt);
	//绑定强化功能
	this->M_UpGradeCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CardUpGrade);
	this->M_Clover_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSelectClover);

	this->M_UpGradeCard_Main_Grade = this->GetWidgetComponent<UImage>(this, "Image_14");
	this->M_UpGradeCard_0_Grade = this->GetWidgetComponent<UImage>(this, "Image_15");
	this->M_UpGradeCard_1_Grade = this->GetWidgetComponent<UImage>(this, "Image_109");
	this->M_UpGradeCard_2_Grade = this->GetWidgetComponent<UImage>(this, "Image_16");
	/*----------------------------------------卡片强化----------------------------------------*/



	/*----------------------------------------卡片转职----------------------------------------*/
	this->M_ChangeJob_0_Image = this->GetWidgetComponent<UImage>(this, "Image_118");
	this->M_ChangeJob_1_Image = this->GetWidgetComponent<UImage>(this, "Image_11");
	this->M_ChangeJob_2_Image = this->GetWidgetComponent<UImage>(this, "Image_12");
	this->M_ChangeJobMainCard_Butt = this->GetWidgetComponent<UButton>(this, "Button_5");
	this->M_ChangeJob_Butt = this->GetWidgetComponent<UButton>(this, "ChangeCardType_Butt");
	this->M_ChangeJob_2_P = this->GetWidgetComponent<UCanvasPanel>(this, "CanvasPanel_4");


	this->M_ChangeJobMainCard_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CancelSelectCardChangeJobs);
	this->M_ChangeJob_Butt->OnClicked.AddDynamic(this, &UUI_PlayerSynthesis::CardChangeJobs);

	/*----------------------------------------卡片转职----------------------------------------*/


	return true;
}

void UUI_PlayerSynthesis::ShowPanel(UCanvasPanel* _UCanvasPanel, uint8 _Index)
{
	UWidgetBase::SetButtonStyle(this->M_MakeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_8.T_PS_8'");
	UWidgetBase::SetButtonStyle(this->M_UpCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_10.T_PS_10'");
	UWidgetBase::SetButtonStyle(this->M_ChangeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_12.T_PS_12'");

	this->M_MakeCardP->SetVisibility(ESlateVisibility::Hidden);
	this->M_UpCardP->SetVisibility(ESlateVisibility::Hidden);
	this->M_ChangeCardP->SetVisibility(ESlateVisibility::Hidden);

	switch (_Index) {
	case 1:UWidgetBase::SetButtonStyle(this->M_UpCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_11.T_PS_11'"); break;
	case 2:UWidgetBase::SetButtonStyle(this->M_ChangeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_13.T_PS_13'"); break;
	default:UWidgetBase::SetButtonStyle(this->M_MakeCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_9.T_PS_9'"); break;
	}

	_UCanvasPanel->SetVisibility(ESlateVisibility::Visible);
}

void UUI_PlayerSynthesis::LoadMaterialData(FMaterialBase* _Item, int32 _Index, int32 _Col, bool _bSelect, const FString& _SelectMaterialsName)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LoadClass<UUI_PlayerBagMaterialGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")));
	Grid->M_CountText = "x" + FString::FormatAsNumber(_Item->M_Count);
	Grid->M_FMaterialBaseData = *_Item;
	UWidgetBase::SetButtonStyle(Grid->GetButton(), _Item->ItemTexturePath);

	//绑定事件
	if (_Item->M_MaterialType == EMaterialType::E_Blueprint)
	{
		Grid->SetUI_PlayerSynthesis(this);
		FScriptDelegate AddFunc;
		AddFunc.BindUFunction(Grid, "AddSynthesisBlueprint");
		Grid->GetButton()->OnClicked.Add(AddFunc);
	}

	this->M_Materials_Box->AddChildToUniformGrid(Grid, (_Index / _Col), _Index - _Col * (_Index / _Col));

	if (_bSelect && _Item->ItemName.Equals(_SelectMaterialsName))
	{
		//模拟点击配方
		Grid->AddSynthesisBlueprint();
	}
}

void UUI_PlayerSynthesis::LoadCardData(FItemCard& _Item, int32 _Grade_, int32 _Index, int32 _Col, bool _IsBind, const FName _BindName, UUniformGridPanel* _ShowRange)
{
	UUI_PlayerBagCardGrid* Grid = CreateWidget<UUI_PlayerBagCardGrid>(this, LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));
	Grid->M_CardTexturePath = _Item.ItemTexturePath;
	Grid->SetFItemCardData(&_Item);
	Grid->SetUI_PlayerSynthesis(this);
	Grid->UpdateButtonTexture(FString::FormatAsNumber(_Item.M_CardPrice));
	Grid->PlayBeginAnim();

	//如果等级限定不是对于-1 并且当前卡片等级低于限定等级  都会直接返回
	if (_Grade_ != -1 && _Item.M_CardGrade < _Grade_)
	{
		Grid->GetButton()->SetIsEnabled(false);
	}

	if (_IsBind)
	{
		//绑定
		FScriptDelegate AddFunc;
		AddFunc.BindUFunction(Grid, _BindName);
		Grid->GetButton()->OnClicked.Add(AddFunc);
	}

	_ShowRange->AddChildToUniformGrid(Grid, (_Index / _Col), _Index - _Col * (_Index / _Col));
}

void UUI_PlayerSynthesis::LoadMaterial_2_Data(FMaterialBase* _Item, int32 _Index, int32 _Col, bool _bBind, FName _BindFunctionName)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this, LoadClass<UUI_PlayerBagMaterialGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")));
	Grid->M_CountText = "x" + FString::FormatAsNumber(_Item->M_Count);
	Grid->SetMaterialData(_Item);
	UWidgetBase::SetButtonStyle(Grid->GetButton(), _Item->ItemTexturePath);

	//绑定事件
	if (_bBind)
	{
		Grid->SetUI_PlayerSynthesis(this);
		FScriptDelegate AddFunc;
		AddFunc.BindUFunction(Grid, _BindFunctionName);
		Grid->GetButton()->OnClicked.Add(AddFunc);
	}

	this->M_Materials_2_Box->AddChildToUniformGrid(Grid, (_Index / _Col), _Index - _Col * (_Index / _Col));
}

void UUI_PlayerSynthesis::ShowMakeCard()
{
	this->ShowPanel(this->M_MakeCardP, 0);
}

void UUI_PlayerSynthesis::ClearPanel()
{
	UGameSystemFunction::CloseUpdate();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
	this->RemoveFromParent();
	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData();
}

void UUI_PlayerSynthesis::ShowUpCard()
{
	UGameSystemFunction::OpenUpdate();

	this->ShowPanel(this->M_UpCardP, 1);
}

void UUI_PlayerSynthesis::ShowChangeCard()
{
	UGameSystemFunction::OpenUpdate();

	this->ShowPanel(this->M_ChangeCardP, 2);
}

void UUI_PlayerSynthesis::LoadMaterials()
{
	this->M_Materials_CardMake_Arrays_Data.Empty();
	this->M_Materials_Box->ClearChildren();

	//卡片数据
	TArray<FMaterialBase>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
	//获取合成配方
	this->M_Materials_CardMake_Arrays_Data.Append(UGameSystemFunction::GetMaterialsArrayByType(_Items_, EMaterialType::E_Blueprint));
	//获取配方材料
	this->M_Materials_CardMake_Arrays_Data.Append(UGameSystemFunction::GetMaterialsArrayByType(_Items_, EMaterialType::E_CardSynthesisMaterial));

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {

		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < this->M_Materials_CardMake_Arrays_Data.Num()) {
			this->LoadMaterialData(this->M_Materials_CardMake_Arrays_Data[this->M_FTimeClip_0.M_Count], this->M_FTimeClip_0.M_Count, 6, true, this->M_MaterialBlueprintName);
			this->M_FTimeClip_0.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		}
		}, 0.01f, true);
}

void UUI_PlayerSynthesis::LoadMaterialsSpices()
{
	//取消香料的选择
	this->M_MakeCard_SpicesGrid = nullptr;
	this->CancelSelectSpices();
	this->LoadMaterials_Type<EMaterialType::E_Spices, 9, true>("AddSynthesisSpicesSlot");
}

void UUI_PlayerSynthesis::LoadMaterialsClover()
{
	this->M_MakeCard_CloverGrid = nullptr;
	this->CancelSelectClover();
	this->LoadMaterials_Type<EMaterialType::E_Clover, 9, true>("AddUpGradeCardCloverSlot");
}

void UUI_PlayerSynthesis::LoadMaterialsCardChangeJobs()
{
	this->LoadMaterials_Type<EMaterialType::E_CardChangeJobMaterial, 9, false>("");
}

void UUI_PlayerSynthesis::LoadCards()
{
	this->M_Cards_Box->ClearChildren();
	this->M_Cards_2_Box->ClearChildren();

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_1.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FItemCard>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
		//判断索引是否有效
		if (this->M_FTimeClip_1.M_Count < _Items_.Num())
		{
			//不能超过背包最大个数
			if (this->M_FTimeClip_1.M_Count >= UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1))
			{
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
			}

			this->LoadCardData(_Items_[this->M_FTimeClip_1.M_Count], -1, this->M_FTimeClip_1.M_Count, 8, false, "", this->M_Cards_Box);
			this->M_FTimeClip_1.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
		}
		}, 0.01f, true);
}

void UUI_PlayerSynthesis::LoadCards_2(int32 _CardGrade, bool _bIsBind, FName _BindFuncName)
{
	this->CancelSelectCardChangeJobs();
	for (int32 i = 0; i < 4; i++)
		this->CancelSelectCard(i);

	this->M_Cards_Box->ClearChildren();
	this->M_Cards_2_Box->ClearChildren();

	// "AddSynthesisUpGrade"

	this->M_CardGrade = _CardGrade;
	this->M_IsBindFunction = _bIsBind;
	this->M_BindFunctionName = _BindFuncName;

	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_1.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FItemCard>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
		//判断索引是否有效
		if (this->M_FTimeClip_1.M_Count < _Items_.Num())
		{
			//不能超过背包最大个数
			if (this->M_FTimeClip_1.M_Count >= UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetBagCount(1))
			{
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
			}

			this->LoadCardData(_Items_[this->M_FTimeClip_1.M_Count], this->M_CardGrade, this->M_FTimeClip_1.M_Count, 8, this->M_IsBindFunction, this->M_BindFunctionName, this->M_Cards_2_Box);

			this->M_FTimeClip_1.M_Count++;
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_1);
		}
		}, 0.01f, true);
}

void UUI_PlayerSynthesis::CancelSelectMakeCard()
{
	this->M_MaterialsData.Empty();
	//原因:它指向的玩家背包中加载的数据->并非新创建的数据，被指向的数据由玩家角色结构管理器统一管理
	this->M_MaterialBlueprintData = nullptr;

	UWidgetBase::SetImageBrush(this->M_Material_0_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");
	UWidgetBase::SetImageBrush(this->M_Material_1_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");
	UWidgetBase::SetImageBrush(this->M_Material_2_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_17.T_PS_17'");

	UWidgetBase::SetButtonStyle(this->M_SynthesisBlueprint_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_16.T_PS_16'");
}

void UUI_PlayerSynthesis::CancelSelectSpices()
{
	if (this->M_MakeCard_SpicesGrid)
	{
		this->M_MakeCard_SpicesGrid->GetButton()->SetIsEnabled(true);
		//移除香料
		this->M_MakeCard_SpicesGrid = nullptr;
	}
	//还原按钮样式
	UWidgetBase::SetButtonStyle(this->M_Spices_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_18.T_PS_18'");
}

void UUI_PlayerSynthesis::CancelSpicesDestroy()
{
	//清除名称
	this->M_SelectSpicesName = "";

	this->CancelSelectSpices();
}

void UUI_PlayerSynthesis::MakeCard()
{

	if (!this->M_MaterialBlueprintData)
	{
		//提示玩家应该选择卡片合成的配方
		UWidgetBase::CreateTipWidget(TEXT("请道具栏中选择防御卡合成配方"));
		return;
	}

	//查询数据的完整性
	for (FMaterialBase* _ItemDatas : this->M_MaterialsData)
	{
		if (!_ItemDatas)
		{
			//提示玩家——>缺少材料
			UWidgetBase::CreateTipWidget(TEXT("好像不能制作哦，是不是缺少材料了"));
			return;
		}
	}

	//消耗制作材料的配方
	this->M_MaterialBlueprintData->M_Count--;
	//数量为0
	if (this->M_MaterialBlueprintData->M_Count == 0)
	{
		this->M_MaterialBlueprintData->bWaitRemove = true;
	}

	//消耗道具
	for (FMaterialBase* _ItemDatas : this->M_MaterialsData)
	{
		_ItemDatas->M_Count--;
		if (_ItemDatas->M_Count == 0)
			_ItemDatas->bWaitRemove = true;
	}

	//获取配方数据
	//TArray<FMaterial_CardSynthesisBlueprint_Data*> _MaterialData;
	//UPlayerStructManager::GetTableRow<FMaterial_CardSynthesisBlueprint_Data>("DataTable'/Game/Resource/BP/Data/MaterialData/Material_CardSynthesisBlueprint_Data.Material_CardSynthesisBlueprint_Data'", _MaterialData);
	//FCardBlueprint ItemData = UPlayerStructManager::GetDataFromTable_Materials<FCardBlueprint, FMaterial_CardSynthesisBlueprint_Data>(this->M_MaterialBlueprintName, _MaterialData);


	//获取制作成功的卡片数据
	FItemCard _Card;
	UPlayerStructManager::SearchCardFromDataTable(this->M_MaterialBlueprintTargetName, _Card);

	//设置卡片等级（如果存在那么设置卡片等级然后移除）
	if (this->M_MakeCard_SpicesGrid)
	{
		FString ItemName = M_MakeCard_SpicesGrid->GetMaterialData()->ItemName;
		this->M_SelectSpicesName = ItemName;

		FSpicesMaterial _Spices;
		UPlayerStructManager::GetMaterialSourceData<FMaterial_SpicesMaterial_Data, FSpicesMaterial>(ItemName, _Spices, EMaterialType::E_Spices);

		_Card.M_CardGrade = _Spices.M_MakeCardGrade;

		//扣除道具数量
		this->M_MakeCard_SpicesGrid->GetMaterialData()->M_Count -= 5;
		//更新显示数量
		this->M_MakeCard_SpicesGrid->UpdateMaterialsShowCount(FString::FormatAsNumber(this->M_MakeCard_SpicesGrid->GetMaterialData()->M_Count));
		//判断个数
		if (this->M_MakeCard_SpicesGrid->GetMaterialData()->M_Count < 5)
		{
			//如果香料个数==0
			if (this->M_MakeCard_SpicesGrid->GetMaterialData()->M_Count <= 0)
				this->M_MakeCard_SpicesGrid->GetMaterialData()->bWaitRemove = true;
			//将选择的名称移除
			this->M_SelectSpicesName = "";
		}
	}

	//取消香料的选择
	this->CancelSelectSpices();
	//取消卡片选择
	this->CancelSelectMakeCard();

	UWidgetBase::CreateTipWidget(TEXT("制作成功，这样就有新卡片了"), FVector(1.f, 0.f, 1.f));

	//清空待删除的道具
	UGameSystemFunction::ClearWaitingItems<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);

	//更新列表并且再次选中这个配方
	this->LoadMaterials();

	//  UUI_PlayerBagMaterialGrid::SetUI_PlayerSynthesis 将进行设置香料的选择
	//更新列表并且再次选中这个香料
	this->LoadMaterialsSpices();

	//将卡片添加到背包
	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(_Card);

	//更新卡片界面->不需要重新加载->直接添加一个到最后
	this->LoadCards();
}

void UUI_PlayerSynthesis::CancelSelectCard(int32 _Cancel_Index)
{
	this->M_UpGradeCardState[_Cancel_Index] = false;

	//改指针指向的内容统一由卡片格子UI管理
	if (this->M_UpGradeCardGridUI[_Cancel_Index])
		this->M_UpGradeCardGridUI[_Cancel_Index]->GetButton()->SetIsEnabled(true);

	this->M_UpGradeCardGridUI[_Cancel_Index] = nullptr;

	if (_Cancel_Index == 0)
		UWidgetBase::SetButtonStyle(this->M_UpGradeSlot[_Cancel_Index], "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
	else
		UWidgetBase::SetButtonStyle(this->M_UpGradeSlot[_Cancel_Index], "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_20.T_PS_20'");

	if (_Cancel_Index == 0)
		this->M_UpGradeCard_Main_Grade->SetVisibility(ESlateVisibility::Hidden);
	if (_Cancel_Index == 1)
		this->M_UpGradeCard_0_Grade->SetVisibility(ESlateVisibility::Hidden);
	if (_Cancel_Index == 2)
		this->M_UpGradeCard_1_Grade->SetVisibility(ESlateVisibility::Hidden);
	if (_Cancel_Index == 3)
		this->M_UpGradeCard_2_Grade->SetVisibility(ESlateVisibility::Hidden);

	//更新概率
	this->GetUpGradeRate();
}

void UUI_PlayerSynthesis::CancelSelectClover()
{
	if (this->M_MakeCard_CloverGrid)
	{
		this->M_MakeCard_CloverGrid->GetButton()->SetIsEnabled(true);
		this->M_MakeCard_CloverGrid = nullptr;
	}
	UWidgetBase::SetButtonStyle(this->M_Clover_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_25.T_PS_25'");
	//更新概率
	this->GetUpGradeRate();
}

void UUI_PlayerSynthesis::CardUpGrade()
{
	//判断主卡是否存在，主卡不存在则不能强化
	if (!this->M_UpGradeCardState[0])
	{
		//提示玩家请选择主要强化的卡片
		UWidgetBase::CreateTipWidget(TEXT("嗯？好像没有选择[主卡]哦"));

		return;
	}

	//判断主卡等级
	if (this->M_UpGradeCardGridUI[0]->GetFItemCardData()->M_CardGrade == 16)
	{
		//提示玩家主卡等级已经到最高级
		UWidgetBase::CreateTipWidget(TEXT("哇哇哇呜呜呜，这么高的等级，臣妾做不到啊····"));

		return;
	}

	//清空副卡->查询副卡
	int32 _Index = 0;

	bool _UseUpGrade = false;

	for (bool& _Condition : this->M_UpGradeCardState)
	{

		//主卡保留
		if (_Index == 0)
		{
			_Index++;
			continue;
		}


		if (_Condition)
		{
			//标记
			this->M_UpGradeCardGridUI[_Index]->GetFItemCardData()->bWaitRemove = true;
			_UseUpGrade = true;
		}

		_Index++;
	}

	if (!_UseUpGrade)
	{
		//提示玩家添加副卡
		UWidgetBase::CreateTipWidget(TEXT("嗯？选择一张副卡试试"));
		return;
	}

	if (UGameSystemFunction::GetRange(this->GetUpGradeRate(), 100))
	{
		//等级提升（强化成功）
		if (this->M_UpGradeCardGridUI[0]->GetFItemCardData()->M_CardGrade < 16)
			this->M_UpGradeCardGridUI[0]->GetFItemCardData()->M_CardGrade++;
		UWidgetBase::CreateTipWidget(TEXT("强化成功"), FVector(1.f, 0.f, 1.f));
	}
	else {
		//等级提升（强化失败）
		if (this->M_UpGradeCardGridUI[0]->GetFItemCardData()->M_CardGrade >= 6)
			this->M_UpGradeCardGridUI[0]->GetFItemCardData()->M_CardGrade--;
		UWidgetBase::CreateTipWidget(TEXT("不够好运,强化失败"), FVector(1.f, 0.f, 1.f));
	}


	if (this->M_MakeCard_CloverGrid)
	{
		this->M_MakeCard_CloverGrid->GetMaterialData()->M_Count--;
		if (this->M_MakeCard_CloverGrid->GetMaterialData()->M_Count == 0)
		{
			this->M_MakeCard_CloverGrid->GetMaterialData()->bWaitRemove = true;
			UGameSystemFunction::ClearWaitingItems<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);
		}
		//取消对四叶草的选中
		this->CancelSelectClover();
	}

	//清理状态
	for (int32 i = 0; i < 4; i++)
		this->CancelSelectCard(i);

	//清空待删除的道具
	UGameSystemFunction::ClearWaitingItems<FItemCard>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card);

	//重新加载卡片
	this->LoadCards_2(-1, true, "AddSynthesisUpGrade");

	//加载四叶草
	this->LoadMaterialsClover();

	//保存存档
	UGameSystemFunction::SaveCurrentPlayerData();
}

float UUI_PlayerSynthesis::GetUpGradeRate()
{
	float Rate = 0.0f;
	int32 _MainCardGrade = 0;
	int32 _Index_ = 0;

	for (UUI_PlayerBagCardGrid* Grid : this->M_UpGradeCardGridUI)
	{
		if (_Index_ == 0)
		{
			if (!Grid)
			{
				this->M_UpGradeCardRateText = TEXT("0%");
				return 0.f;
			}
			//获取主卡等级
			_MainCardGrade = Grid->GetFItemCardData()->M_CardGrade;
			_Index_++;
			continue;
		}

		if (Grid)
		{
			int32 Y = (_MainCardGrade + Grid->GetFItemCardData()->M_CardGrade);
			int32 X = (_MainCardGrade - Grid->GetFItemCardData()->M_CardGrade);

			if (X == 0)
			{
				//副卡主卡同星
				if (Y - 1 < 0)
					Rate += 100.f;
				else
					Rate += this->M_UpGradeRate_0_Arrays[(Y - 1) / 2] / Y;
			}
			else if (X == 1) {
				//副卡小于主卡1星
				Rate += this->M_UpGradeRate_1_Arrays[(Y - 1) / 2] / Y;
			}
			else if (X == 2) {
				//副卡小于主卡2星
				Rate += this->M_UpGradeRate_2_Arrays[(Y - 2) / 2] / Y;
			}
			else if (X < 0)
			{
				//高等级大于主卡  x 星
				X = X / -1;
				Rate += 30.f * (X + 1.2f);
			}
			else {
				//副卡低于主卡3星及以上
				Rate += 0.f;
			}

		}

		_Index_++;
	}

	//是否有四叶草
	if (this->M_MakeCard_CloverGrid)
	{

		FCloverMaterial _Clover;
		UPlayerStructManager::GetMaterialSourceData<FMaterial_CloverMaterial_Data, FCloverMaterial>(this->M_MakeCard_CloverGrid->GetMaterialData()->ItemName, _Clover, EMaterialType::E_Clover);

		//加上四叶草带来的祝福倍率
		Rate *= _Clover.M_UpGrateRate;
	}

	if (Rate > 100.f)
		Rate = 100.f;

	this->M_UpGradeCardRateText = FString::FormatAsNumber((int32)Rate) + "%";

	/*

	卡片概率计算公式:  基数 / (主卡+副卡)的等级;
	副卡比主卡等级低n就会使用不同的基数进行计算

	同星 基数给定值:{ 0,200,360,480,560,400,420,420,400,360,300,220,192,208,224,240,0 }
	低1星 基数给定值:{100,300,440,420,423,429,403,390,374,285,273,376,250,270,361,248 }
	低2星 基数给定值:{0,0,440,420,423,429,403,390,374,285,273,376,250,270,361,248 }
	如果副卡高于主卡->计算公式:30.0 * (X(副卡高于主卡的等级) + 1.2) = 最终概率

	*/

	return Rate;
}

void UUI_PlayerSynthesis::CancelSelectCardChangeJobs()
{
	this->M_CardChangeJobTargetName = "";
	this->M_CardChangeJobMaterialsIndex.Empty();
	this->M_CardChangeJobMaterialsArrays.Empty();

	if (this->M_CardChangeJobGrid)
	{
		this->M_CardChangeJobGrid->GetButton()->SetIsEnabled(true);
		this->M_CardChangeJobGrid = nullptr;
	}

	UWidgetBase::SetImageBrush(this->M_ChangeJob_0_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_22.T_PS_22'");
	UWidgetBase::SetImageBrush(this->M_ChangeJob_1_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_23.T_PS_23'");
	UWidgetBase::SetImageBrush(this->M_ChangeJob_2_Image, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_24.T_PS_24'");
	UWidgetBase::SetButtonStyle(this->M_ChangeJobMainCard_Butt, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerSynthesis/T_PS_21.T_PS_21'");
}

void UUI_PlayerSynthesis::CardChangeJobs()
{
	if (!this->M_CardChangeJobGrid)
		return;

	//材料检测->材料不足则退出
	for (bool& B : this->M_CardChangeJobMaterialsArrays)
		if (!B)
		{
			UWidgetBase::CreateTipWidget(TEXT("缺少转职材料哦,凑齐了转职材料才能够转职哦"));
			return;
		}

	//查询材料
	for (int32& _Index : this->M_CardChangeJobMaterialsIndex)
	{
		UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].M_Count--;
		if (UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].M_Count == 0)
			UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material[_Index].bWaitRemove = true;
	}

	this->M_CardChangeJobGrid->GetFItemCardData()->bWaitRemove = true;

	//清空待删除的道具
	UGameSystemFunction::ClearWaitingItems<FMaterialBase>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material);

	//创建所需卡片
	FItemCard CardData;
	UPlayerStructManager::SearchCardFromDataTable(this->M_CardChangeJobTargetName, CardData, true, this->M_CardChangeJobGrid->GetFItemCardData()->M_FCardChangeJobs.M_ChangeType);
	CardData.M_CardGrade = this->M_CardChangeJobGrid->GetFItemCardData()->M_CardGrade;
	UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card.Emplace(CardData);

	//查询主卡并且删除
	UGameSystemFunction::ClearWaitingItems<FItemCard>(UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card);

	//重新加载列表
	this->LoadCards_2(9, true, "AddSynthesisChangeJobs");

	this->LoadMaterialsCardChangeJobs();

	UWidgetBase::CreateTipWidget(TEXT("恭喜你转职成功"), FVector(1.f, 0.f, 1.f));

	//取消选择
	this->CancelSelectCardChangeJobs();

}
