// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_GameSetting.h"

#include "GameSystem/FVMGameInstance.h"
#include "GameSystem/PlayerStructManager.h"
#include "GameSystem/GameConfigManager.h"
#include <Kismet/GameplayStatics.h>

#include <Components/Slider.h>
#include <Components/CanvasPanel.h>
#include <Components/Button.h>

void UUI_GameSetting::AddPanels()
{
	this->M_AllPanels.Emplace(this->M_UCanvasPanel_0);
}

void UUI_GameSetting::HiddenPanel()
{
	for (auto Panels : this->M_AllPanels)
	{
		//隐藏所有界面除了第一个
		Panels->SetVisibility(ESlateVisibility::Hidden);
	}
}


void UUI_GameSetting::ShowPanel(const int32& _Index)
{
	this->HiddenPanel();
	this->M_AllPanels[_Index]->SetVisibility(ESlateVisibility::Visible);
}

bool UUI_GameSetting::Initialize()
{
	if (!Super::Initialize())
		return false;

	//初始化界面关闭按钮
	this->M_UButton_ClosePanel = this->GetWidgetComponent<UButton>(this, "But_ClosePanel");
	//初始化点击事件
	this->M_UButton_ClosePanel->OnClicked.AddDynamic(this, &UUI_GameSetting::Butt_ClosePanel);


	/*----------------------------------------第一个界面----------------------------------------*/
	//初始化音乐进度条
	this->M_UCanvasPanel_0 = this->GetWidgetComponent<UCanvasPanel>(this, "SettingPanel_0");
	this->M_USlider_BGM = this->GetWidgetComponent<USlider>(this, "BGM_Slider");
	this->M_USlider_BGM_S = this->GetWidgetComponent<USlider>(this, "BGM_S_Slider");
	/*----------------------------------------第一个界面----------------------------------------*/

	//添加界面到数组
	this->AddPanels();
	return true;
}

void UUI_GameSetting::Butt_ClosePanel()
{
	//保存数据
	UFVMGameInstance::GetFVMGameInstance()->SaveDataToGameConfigManager(UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager());

	this->RemoveFromParent();
}

void UUI_GameSetting::OpenPanel_0()
{
	this->ShowPanel(0);
	//读取配置写入数据
	const FMusic& Music_ = UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic;
	this->M_USlider_BGM->SetValue(Music_.M_BGM_Value);
	this->M_USlider_BGM_S->SetValue(Music_.M_BGM_S_Value);
}

void UUI_GameSetting::USliderValueChanged()
{
	//读取配置写入数据并且保存数据
	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_BGM_Value = this->M_USlider_BGM->GetValue();
	UFVMGameInstance::GetFVMGameInstance()->GetGameConfigManager()->M_FMusic.M_BGM_S_Value = this->M_USlider_BGM_S->GetValue();
}
