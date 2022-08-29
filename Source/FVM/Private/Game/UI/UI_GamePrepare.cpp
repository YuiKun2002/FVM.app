// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GamePrepare.h"
#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"

#include "Game/UI/UI_PlayerBagCardGrid.h"
#include <Components/UniformGridPanel.h>
#include <Components/HorizontalBox.h>

#include <Kismet/KismetSystemLibrary.h>

UUI_GamePrepare* UUI_GamePrepare::M_GamePrepareStatic = nullptr;

bool UUI_GamePrepare::Initialize()
{
	if (!Super::Initialize())
		return false;

	//赋值静态变量
	UUI_GamePrepare::M_GamePrepareStatic = this;
	UE_LOG(LogTemp, Warning, TEXT("[UUI_GamePrepare::Initialize]: 赋值静态M_GamePrepareStatic"));


	this->M_CardGrid = this->GetWidgetComponent<UUniformGridPanel>(this, "CardGrid");
	this->M_SelectCardList = this->GetWidgetComponent<UHorizontalBox>(this, "SelectCardList");

	return true;
}

void UUI_GamePrepare::SelectCard(const FString& _CardName, const FItemCard& _CardData)
{
	for (auto Widget : this->M_BagGrid)
	{
		//寻找到你当前点击的卡片
		if (Widget->GetFItemCardData()->ItemName.Equals(_CardName))
		{
			//禁用相关按钮
			Widget->GetButton()->SetIsEnabled(false);
		}
	}

	UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));
	M_TempCardGrid->SetFItemCardData(const_cast<FItemCard*>(&_CardData));
	M_TempCardGrid->M_CardTexturePath = _CardData.ItemTexturePath;
	M_TempCardGrid->UpdateButtonTexture(FString::FormatAsNumber(_CardData.M_CardPrice));
	M_TempCardGrid->PlayBeginAnim();

	//绑定函数 点击之后显示细节面板
	FScriptDelegate CallFunc;
	CallFunc.BindUFunction(M_TempCardGrid, "RemoveCurrentSelectCard");
	M_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
	//添加到列表
	this->M_SelectCardList->AddChildToHorizontalBox(M_TempCardGrid);
	//选择数量+1
	this->M_SelectCardNum++;

	//UE_LOG(LogTemp, Error, TEXT("%s  %d"), __FUNCTION__, _CardData.M_CardLayer);
}

void UUI_GamePrepare::SetCardEnable(const TArray<FString>& _Names, bool _bEnable)
{
	if (!_Names.Num())
		return;

	for (auto Widget : this->M_BagGrid)
	{
		for (auto NamePP = _Names.CreateConstIterator(); NamePP; NamePP++)
		{
			//寻找到你当前点击的卡片
			if (Widget->GetFItemCardData()->ItemName.Equals(*NamePP))
			{
				//禁用相关按钮
				if (_bEnable)
					Widget->GetButton()->SetIsEnabled(true);
				else
					Widget->GetButton()->SetIsEnabled(false);
				break;
			}
		}
	}
}

void UUI_GamePrepare::CancelCardNum()
{
	this->M_SelectCardNum--;
}

int32 UUI_GamePrepare::GetSelectCardNums()
{
	return this->M_SelectCardNum;
}

void UUI_GamePrepare::LoadCardList()
{
	this->M_CardGrid->ClearChildren();

	this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
	this->M_FTimeClip.M_Count = 0;

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FItemCard>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
		//判断索引是否有效
		if (this->M_FTimeClip.M_Count < _Items_.Num())
		{
			UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));
			M_TempCardGrid->M_CardTexturePath = _Items_[this->M_FTimeClip.M_Count].ItemTexturePath;
			M_TempCardGrid->SetFItemCardData(&_Items_[this->M_FTimeClip.M_Count]);
			M_TempCardGrid->UpdateButtonTexture(FString::FormatAsNumber(_Items_[this->M_FTimeClip.M_Count].M_CardPrice));
			M_TempCardGrid->PlayBeginAnim();

			//查询当前创建的新卡片是否被玩家选择(如果选择了则禁用按钮功能)
			for (auto CardDatas : this->M_CardDatas_Copy)
			{
				if ((CardDatas).ItemName.Equals(_Items_[this->M_FTimeClip.M_Count].ItemName))
				{
					M_TempCardGrid->GetButton()->SetIsEnabled(false);
					break;
				}
			}

			//绑定函数 点击之后显示细节面板
			FScriptDelegate CallFunc;
			CallFunc.BindUFunction(M_TempCardGrid, "SelectCurrentCard");
			M_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
			//将widget添加到格子中
			this->M_BagGrid.Emplace(M_TempCardGrid);
			this->M_CardGrid->AddChildToUniformGrid(M_TempCardGrid, (this->M_FTimeClip.M_Count / 12), this->M_FTimeClip.M_Count - 12 * (this->M_FTimeClip.M_Count / 12));
			this->M_FTimeClip.M_Count++;
		}
		else {
			this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
			this->M_FTimeClip.M_Count = 0;
		}

		}, 0.02f, true);
}

void UUI_GamePrepare::CloseCurrentPanel()
{
	this->RemoveFromParent();

	//将选卡的数据保存到游戏配置结构当中去
	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_PlayerGamePrepareSelectCardData = this->M_CardDatas_Copy;
	UFVMGameInstance::GetFVMGameInstance()->SaveDataToGameConfigManager();

	this->GetWorld()->GetTimerManager().ClearTimer(this->M_FTimeClip.M_FTimerHandle);
	this->M_FTimeClip.M_Count = 0;
}
