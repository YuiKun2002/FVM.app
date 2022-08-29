// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerBagCardDetail.h"
#include "Game/UI/UI_GamePrepare.h"
#include "Game/UI/UI_PlayerBag.h"
#include "Game/UI/UI_PlayerSynthesis.h"
#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Math/Color.h>

bool UUI_PlayerBagCardGrid::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_CardButton = this->GetWidgetComponent<UButton>(this, "Card");
	this->M_CardGradeImage = this->GetWidgetComponent<UImage>(this, "Grade");

	return true;
}

FOnButtonClickedEvent& UUI_PlayerBagCardGrid::GetButtonClickEvent()
{
	return this->M_CardButton->OnClicked;
}

void UUI_PlayerBagCardGrid::SetFItemCardData(FItemCard* const _FItemCardData)
{
	this->M_FItemCard = _FItemCardData;
}

void UUI_PlayerBagCardGrid::SetUI_PlayerSynthesis(UUI_PlayerSynthesis* const _PlayerSynthesis)
{
	this->M_UI_PlayerSynthesis = _PlayerSynthesis;
}

FItemCard* const UUI_PlayerBagCardGrid::GetFItemCardData()
{
	return this->M_FItemCard;
}

UButton* UUI_PlayerBagCardGrid::GetButton()
{
	return this->M_CardButton;
}

void UUI_PlayerBagCardGrid::ShowCardDetails()
{
	if (!this->GetFItemCardData())
		return;

	//细节界面
	UUI_PlayerBagCardDetail* M_UI_PlayerBagCardDetail = CreateWidget<UUI_PlayerBagCardDetail>(this, LoadClass<UUI_PlayerBagCardDetail>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardDetail.BPUI_PlayerBagCardDetail_C'")));
	M_UI_PlayerBagCardDetail->M_PlayerUIBag = this->M_PlayerUIBag;
	M_UI_PlayerBagCardDetail->M_FItemCard = this->M_FItemCard;
	M_UI_PlayerBagCardDetail->ItemName = M_FItemCard->ItemName;
	M_UI_PlayerBagCardDetail->ItemDescrible = M_FItemCard->ItemDescrible;
	M_UI_PlayerBagCardDetail->ItemDescrible_2 = M_FItemCard->ItemDescrible_2;
	M_UI_PlayerBagCardDetail->M_CardHP = FString(TEXT("生命值:")) + FString::FormatAsNumber(M_FItemCard->M_CardHP);
	M_UI_PlayerBagCardDetail->M_CardPrice = FString(TEXT("火苗:")) + FString::FormatAsNumber(M_FItemCard->M_CardPrice);
	M_UI_PlayerBagCardDetail->M_IsChange = M_FItemCard->M_FCardChangeJobs.M_IsChange ? FString(TEXT("转职:可以转职")) : FString(TEXT("转职:不可转职"));
	M_UI_PlayerBagCardDetail->M_ChangeMaterialsName = FString(TEXT("转职对象:")) + M_FItemCard->M_FCardChangeJobs.M_ChangeMaterialsName;
	M_UI_PlayerBagCardDetail->M_CardColdDown = FString(TEXT("冷却时间:")) + FString::SanitizeFloat(M_FItemCard->M_CardColdDown, 2);
	M_UI_PlayerBagCardDetail->AddToViewport();
}

void UUI_PlayerBagCardGrid::SelectCurrentCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		if (UUI_GamePrepare::M_GamePrepareStatic->GetSelectCardNums() > 13)
		{
			UWidgetBase::CreateTipWidget(TEXT("最多选择14张防御卡哦!"));
			return;
		}

		UUI_GamePrepare::M_GamePrepareStatic->SelectCard(this->M_FItemCard->ItemName, *M_FItemCard);
		UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.Emplace(*this->M_FItemCard);
	}
}

void UUI_PlayerBagCardGrid::RemoveCurrentSelectCard()
{
	if (UUI_GamePrepare::M_GamePrepareStatic)
	{
		this->RemoveFromParent();

		for (auto FCardDataPP = UUI_GamePrepare::M_GamePrepareStatic->M_CardDatas_Copy.CreateIterator(); FCardDataPP; FCardDataPP++)
		{
			if (this->M_FItemCard->ItemName.Equals((*FCardDataPP).ItemName))
			{
				FCardDataPP.RemoveCurrent();
				break;
			}
		}

		TArray<FString> Names = { M_FItemCard->ItemName };
		UUI_GamePrepare::M_GamePrepareStatic->SetCardEnable(Names, true);
		UUI_GamePrepare::M_GamePrepareStatic->CancelCardNum();
	}
}

void UUI_PlayerBagCardGrid::AddSynthesisUpGrade()
{
	int32 _Use_Index = 0;
	bool _IsUse = false;

	//判断卡槽是否可用
	for (bool& Number : this->M_UI_PlayerSynthesis->M_UpGradeCardState)
	{
		if (!Number)
		{
			Number = true;
			_IsUse = true;
			break;
		}
		_Use_Index++;
	}

	//如果不能使用退出
	if (!_IsUse)
		return;

	auto SetImage = [&](UImage* _img)->void {
		_img->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UWidgetBase::SetImageBrush(_img, UGameSystemFunction::GetCardGradeImagePath(this->M_FItemCard->M_CardGrade));
	};

	if (_Use_Index == 0)
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_Main_Grade);
	if (_Use_Index == 1)
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_0_Grade);
	if (_Use_Index == 2)
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_1_Grade);
	if (_Use_Index == 3)
		SetImage(this->M_UI_PlayerSynthesis->M_UpGradeCard_2_Grade);


	this->M_UI_PlayerSynthesis->M_UpGradeCardGridUI[_Use_Index] = this;
	UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_UpGradeSlot[_Use_Index], this->M_FItemCard->ItemTexturePath);
	this->GetButton()->SetIsEnabled(false);

	//计算概率
	this->M_UI_PlayerSynthesis->GetUpGradeRate();
}

void UUI_PlayerBagCardGrid::AddSynthesisChangeJobs()
{
	//如果已经有选择了则取消选择上一个
	if (this->M_UI_PlayerSynthesis->M_CardChangeJobGrid)
		this->M_UI_PlayerSynthesis->M_CardChangeJobGrid->GetButton()->SetIsEnabled(true);

	//查询当前卡片是否能转职
	FItemCard Card;
	UPlayerStructManager::SearchCardFromDataTable(this->M_FItemCard->ItemName, Card, true, this->M_FItemCard->M_ECardType);

	if (this->M_FItemCard->M_FCardChangeJobs.M_IsChange != Card.M_FCardChangeJobs.M_IsChange)
	{
		this->M_FItemCard->M_FCardChangeJobs = Card.M_FCardChangeJobs;
	}

	//判断当前卡片是否能转职  不能转职则退出
	if (!this->M_FItemCard->M_FCardChangeJobs.M_IsChange)
	{
		UWidgetBase::CreateTipWidget(TEXT("当前卡片不能转职哦"));
		return;
	}

	//如果转职等级不够则退出
	if (this->M_FItemCard->M_CardGrade < this->M_FItemCard->M_FCardChangeJobs.M_ChangeGrade)
	{
		FString Tip = FString(TEXT("哎呀，这张防御卡的等级需要达到")) + FString::FormatAsNumber(this->M_FItemCard->M_FCardChangeJobs.M_ChangeGrade) + TEXT("级才可以转职哦");
		UWidgetBase::CreateTipWidget(Tip);
		return;
	}

	this->M_UI_PlayerSynthesis->M_CardChangeJobGrid = this;

	//将转职目标的卡片赋予合成屋成员 临时保存
	this->M_UI_PlayerSynthesis->M_CardChangeJobTargetName = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterialsName;

	//从背包寻找材料
	TArray<FMaterialBase>& _Materials = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
	//需要的材料名称->每当查询成功一次就会移除一位元素剩余的则是未查询成功的
	TArray<FString> _NeedMaterialsName = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials;
	//物品的名称
	TMap<FString, bool> _MaterialsName;
	//图片路径
	TArray<FString> _MaterialsTexturePath_;

	//获取图片路径
	FMaterialBase Data;
	int32 M_Count = this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials.Num();

	//寻找并添加图片路径
	for (int32 I = 0; I < M_Count; I++)
	{
		UPlayerStructManager::SearchMaterailFromDataTable(this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials[I], Data, true, EMaterialType::E_CardChangeJobMaterial);
		_MaterialsName.Emplace(Data.ItemName, false);
		_MaterialsTexturePath_.Emplace(Data.ItemTexturePath);
	}

	//计数索引
	int32 _ItemIndex = 0;
	//查询计数
	int32 _ItemSuccessCount = 0;


	for (FMaterialBase& Item : _Materials)
	{
		for (auto PP = _NeedMaterialsName.CreateConstIterator(); PP; PP++)
		{
			if (Item.ItemName.Equals(*PP))
			{
				//添加状态
				this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsArrays.Emplace(true);
				//添加索引
				this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsIndex.Emplace(_ItemIndex);
				//修改图片状态
				*(_MaterialsName.Find(Item.ItemName)) = true;
				//移除
				_NeedMaterialsName.RemoveAt(PP.GetIndex());
				//更新计数
				_ItemSuccessCount++;
				break;
			}
		}

		//当材料全部被找到之后将直接退出循环
		if (_NeedMaterialsName.Num() == 0)
			break;

		_ItemIndex++;
	}

	//添加剩余状态
	for (FString Name : _NeedMaterialsName)
		this->M_UI_PlayerSynthesis->M_CardChangeJobMaterialsArrays.Emplace(false);

	//显示转职C界面
	if (this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials.Num() > 2)
		this->M_UI_PlayerSynthesis->M_ChangeJob_2_P->SetVisibility(ESlateVisibility::Visible);
	else
		this->M_UI_PlayerSynthesis->M_ChangeJob_2_P->SetVisibility(ESlateVisibility::Hidden);

	//设置图片样式
	auto SetImage = [&](UImage* _Img, int32 _Index)->void {
		bool* State = _MaterialsName.Find(this->M_FItemCard->M_FCardChangeJobs.M_ChangeMaterials[_Index]);
		FString Path = _MaterialsTexturePath_[_Index];
		UWidgetBase::SetImageBrush(_Img, Path, *State ? FVector(1.f) : FVector(0.5f));
	};

	//设置第一张图片
	if (M_Count > 0)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_0_Image, 0);
	//设置第二张图片
	if (M_Count > 1)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_1_Image, 1);
	//设置第三张图片
	if (M_Count > 2)
		SetImage(this->M_UI_PlayerSynthesis->M_ChangeJob_2_Image, 2);

	//设置主卡按钮的图片
	UWidgetBase::SetButtonStyle(this->M_UI_PlayerSynthesis->M_ChangeJobMainCard_Butt, this->M_FItemCard->ItemTexturePath);


	this->M_CardButton->SetIsEnabled(false);
}

void UUI_PlayerBagCardGrid::UpdateButtonTexture(const FString& _Price)
{
	//资源
	FSlateBrush Brush, GradeBrush;
	Brush.SetResourceObject(this->LoadTexture2D(this->M_CardTexturePath));
	GradeBrush.SetResourceObject(this->LoadTexture2D(FString("Texture2D'/Game/Resource/Texture/CardGrade/") + FString::FormatAsNumber(this->M_FItemCard->M_CardGrade) + (".") + FString::FormatAsNumber(this->M_FItemCard->M_CardGrade) + ("'")));
	Brush.DrawAs = ESlateBrushDrawType::Image;
	GradeBrush.DrawAs = ESlateBrushDrawType::Image;

	//设置按钮不同事件时图片颜色深浅
	FButtonStyle Style;
	Style.SetNormal(Brush);

	Brush.TintColor = FLinearColor(FVector4(.7f, .7f, .7f, 1.f));
	Style.SetHovered(Brush);

	Brush.TintColor = FLinearColor(FVector4(.5f, .5f, .5f, 1.f));
	Style.SetPressed(Brush);

	this->M_CardButton->SetStyle(Style);
	this->M_CardPriceText = M_FItemCard->M_CardPriceAutoUp ? FText::FromString(_Price + "+") : FText::FromString(_Price);

	//如果卡片等级为0则不显示
	if (this->M_FItemCard->M_CardGrade == 0)
	{
		this->M_CardGradeImage->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		this->M_CardGradeImage->SetBrush(GradeBrush);

	}
}

UTexture2D* UUI_PlayerBagCardGrid::GetTexture()
{
	return LoadObject<UTexture2D>(0, *this->M_CardTexturePath);
}

void UUI_PlayerBagCardGrid::PlayBeginAnim()
{
	if (this->GetWidgetTreeOwningClass()->Animations.Num())
	{
		this->PlayAnimation(this->GetWidgetTreeOwningClass()->Animations[0]);
	}
}
