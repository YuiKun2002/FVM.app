#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetBase.h"
#include "UI_GameSetting.generated.h"

class USlider;
class UButton;
class UCanvasPanel;

UCLASS()
class FVM_API UUI_GameSetting : public UWidgetBase
{
	GENERATED_BODY()
private:
	//用于存储所有界面
	UPROPERTY()
		TArray<UCanvasPanel*> M_AllPanels;
	//界面关闭按钮
	UPROPERTY()
		UButton* M_UButton_ClosePanel = nullptr;
	/*----------------------------------------第一个界面----------------------------------------*/
	UPROPERTY()
		UCanvasPanel* M_UCanvasPanel_0 = nullptr;
	//背景音乐
	UPROPERTY()
		USlider* M_USlider_BGM = nullptr;
	//背景音效
	UPROPERTY()
		USlider* M_USlider_BGM_S = nullptr;
	/*----------------------------------------背景音乐----------------------------------------*/
private:
	void AddPanels();
	void HiddenPanel();
	void ClosePanel();
	void ShowPanel(const int32& _Index);
public:

	virtual bool Initialize() override;
	//按钮-关闭界面
	UFUNCTION()
		void Butt_ClosePanel();
	//打开第一个界面
	UFUNCTION(BlueprintCallable)
		void OpenPanel_0();
public:
	//音乐音效进度条的值发生改变时
	UFUNCTION(BlueprintCallable)
		void USliderValueChanged();
};
