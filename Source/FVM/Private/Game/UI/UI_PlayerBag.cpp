// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UI/UI_PlayerBag.h"

#include "GameSystem/FVMGameInstance.h"

#include "Game/UI/UI_PlayerBagCardGrid.h"
#include "Game/UI/UI_PlayerBagEquipmentGrid.h"
#include "Game/UI/UI_PlayerBagMaterialGrid.h"
#include "Data/EquipmentDataSturct.h"

#include "Game/UI/UI_PlayerBagRemove.h"

#include <Components/UniformGridPanel.h>
#include <Components/CanvasPanel.h>
#include <Components/Image.h>

#include "Game/UI/Tips/UI_ShopPayTip.h"
#include <Components/EditableTextBox.h>

#include <Kismet/KismetSystemLibrary.h>



bool UUI_PlayerBag::Initialize()
{
	if (!Super::Initialize())
		return false;

	this->M_UUniformGridPanel = this->GetWidgetComponent<UUniformGridPanel>(this, "PlayerItemList");
	this->M_PlayerBagContent = this->GetWidgetComponent<UUniformGridPanel>(this, "Bag_Content");
	this->M_PlayerBagSelectPanel = this->GetWidgetComponent<UCanvasPanel>(this, "PlayerBagSelectPanel");

	//加载背包格子的图片
	this->M_BagCountImage_1 = this->GetWidgetComponent<UImage>(this, "Image");
	this->M_BagCountImage_2 = this->GetWidgetComponent<UImage>(this, "Image_1");
	this->M_BagCountImage_3 = this->GetWidgetComponent<UImage>(this, "Image_2");
	this->M_BagCountImage_4 = this->GetWidgetComponent<UImage>(this, "Image_3");

	//加载背包格子的按钮
	this->M_BagGridButton_1 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_1");
	this->M_BagGridButton_2 = this->GetWidgetComponent<UButton>(this, "Bag_Grid");
	this->M_BagGridButton_3 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_2");
	this->M_BagGridButton_4 = this->GetWidgetComponent<UButton>(this, "Bag_Grid_3");

	//加背包物品删除按钮
	this->M_BagItemUniformRemoveButton = this->GetWidgetComponent<UButton>(this, "RemoveItems");
	this->M_BagItemUniformRemoveButton->OnClicked.AddDynamic(this, &UUI_PlayerBag::OpenRemovePanel);


	this->M_BagGridButton_1->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_1_Func);
	this->M_BagGridButton_2->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_2_Func);
	this->M_BagGridButton_3->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_3_Func);
	this->M_BagGridButton_4->OnClicked.AddDynamic(this, &UUI_PlayerBag::BagGridButton_4_Func);

	return true;
}

int32 UUI_PlayerBag::SetBagGridStyle(int32 M_Count, const TArray<FPlayerBagGird>& _BagGrid)
{

	this->SetImageBrush(this->M_BagCountImage_1, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_2, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_3, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");
	this->SetImageBrush(this->M_BagCountImage_4, "Texture2D'/Game/Resource/Texture/UI/Game/PlayerBag/T_PB_10.T_PB_10'");

	this->M_BagCount_Text_1 = FString();
	this->M_BagCount_Text_2 = FString();
	this->M_BagCount_Text_3 = FString();
	this->M_BagCount_Text_4 = FString();

	int32 GridCount = 150;

	if (M_Count > 0)
	{
		this->M_BagCount_Text_1 = FString::FormatAsNumber(_BagGrid[0].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_1, _BagGrid[0].M_GridImagePath);
		GridCount += _BagGrid[0].M_GridCount;
	}
	if (M_Count > 1)
	{
		this->M_BagCount_Text_2 = FString::FormatAsNumber(_BagGrid[1].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_2, _BagGrid[1].M_GridImagePath);
		GridCount += _BagGrid[1].M_GridCount;
	}
	if (M_Count > 2)
	{
		this->M_BagCount_Text_3 = FString::FormatAsNumber(_BagGrid[2].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_3, _BagGrid[2].M_GridImagePath);
		GridCount += _BagGrid[2].M_GridCount;
	}
	if (M_Count > 3)
	{
		this->M_BagCount_Text_4 = FString::FormatAsNumber(_BagGrid[3].M_GridCount);
		this->SetImageBrush(this->M_BagCountImage_4, _BagGrid[3].M_GridImagePath);
		GridCount += _BagGrid[3].M_GridCount;
	}

	return GridCount;
}

void UUI_PlayerBag::Clear()
{
	//清除定时器
	UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
	//界面索引
	this->M_Panel_Index = 0;
	//移除所有界面
	this->M_UUniformGridPanel->ClearChildren();
}

void UUI_PlayerBag::LoadCard(TArray<FItemCard>& _Items_)
{
	UUI_PlayerBagCardGrid* M_TempCardGrid = CreateWidget<UUI_PlayerBagCardGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagCardGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagCardGrid.BPUI_PlayerBagCardGrid_C'")));
	M_TempCardGrid->M_PlayerUIBag = this;
	M_TempCardGrid->M_CardTexturePath = _Items_[this->M_FTimeClip_0.M_Count].ItemTexturePath;
	M_TempCardGrid->SetFItemCardData(&_Items_[this->M_FTimeClip_0.M_Count]);
	M_TempCardGrid->UpdateButtonTexture(FString::FormatAsNumber(_Items_[this->M_FTimeClip_0.M_Count].M_CardPrice));
	M_TempCardGrid->PlayBeginAnim();
	//绑定函数 点击之后显示细节面板
	FScriptDelegate CallFunc;
	CallFunc.BindUFunction(M_TempCardGrid, "ShowCardDetails");
	M_TempCardGrid->GetButtonClickEvent().Add(CallFunc);
	//最好将widget添加到格子中
	this->M_UUniformGridPanel->AddChildToUniformGrid(M_TempCardGrid, (this->M_FTimeClip_0.M_Count / 8), this->M_FTimeClip_0.M_Count - 8 * (this->M_FTimeClip_0.M_Count / 8));
}

void UUI_PlayerBag::LoadEquipment(TArray<FEquipmentBase>& _Items_)
{
	UUI_PlayerBagEquipmentGrid* Grid = CreateWidget<UUI_PlayerBagEquipmentGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagEquipmentGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagEquipmentGrid.BPUI_PlayerBagEquipmentGrid_C'")));
	Grid->SetItemName(_Items_[this->M_FTimeClip_0.M_Count].ItemName);
	UWidgetBase::SetButtonStyle(Grid->GetButton(), _Items_[this->M_FTimeClip_0.M_Count].ItemTexturePath);

	if (_Items_[this->M_FTimeClip_0.M_Count].M_IsOverlap)
		Grid->M_ItemCountText = FString::FormatAsNumber(_Items_[this->M_FTimeClip_0.M_Count].M_Count);
	else
		Grid->M_ItemCountText = FString();

	//判断当前道具是giftBox则添加按钮事件
	if (_Items_[this->M_FTimeClip_0.M_Count].M_EquipmentType == EEquipment::E_Gift)
	{
		Grid->M_BagUI = this;
		UWidgetBase::ButtonDelegateBind(Grid->GetButton(), Grid, "CreateNewGiftBox");
	}

	this->M_UUniformGridPanel->AddChildToUniformGrid(Grid, (this->M_FTimeClip_0.M_Count / 6), this->M_FTimeClip_0.M_Count - 6 * (this->M_FTimeClip_0.M_Count / 6));
}

void UUI_PlayerBag::LoadMaterial(TArray<FMaterialBase>& _Items_)
{
	UUI_PlayerBagMaterialGrid* Grid = CreateWidget<UUI_PlayerBagMaterialGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagMaterialGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagMaterialGrid.BP_PlayerBagMaterialGrid_C'")));
	UWidgetBase::SetButtonStyle(Grid->GetButton(), _Items_[this->M_FTimeClip_0.M_Count].ItemTexturePath);
	Grid->M_CountText = FString::FormatAsNumber(_Items_[this->M_FTimeClip_0.M_Count].M_Count);
	this->M_UUniformGridPanel->AddChildToUniformGrid(Grid, (this->M_FTimeClip_0.M_Count / 6), this->M_FTimeClip_0.M_Count - 6 * (this->M_FTimeClip_0.M_Count / 6));
}

void UUI_PlayerBag::LoadEquipmentBagToPlayerBag(UUniformGridPanel* _Uniform, int32 _Col)
{
	UUI_PlayerBagEquipmentGrid* Grid = CreateWidget<UUI_PlayerBagEquipmentGrid>(this->GetWorld(), LoadClass<UUI_PlayerBagEquipmentGrid>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BPUI_PlayerBagEquipmentGrid.BPUI_PlayerBagEquipmentGrid_C'")));

	//让结构指向背包中的数据
	Grid->M_EuipmentData = this->M_PlayerBagBuffer[this->M_FTimeClip_0.M_Count];
	//当前界面赋予格子
	Grid->M_BagUI = this;

	UWidgetBase::SetButtonStyle(Grid->GetButton(), this->M_PlayerBagBuffer[this->M_FTimeClip_0.M_Count]->ItemTexturePath);

	//绑定事件
	FScriptDelegate Func;
	Func.BindUFunction(Grid, "AttachToBag");
	Grid->GetButton()->OnClicked.Add(Func);

	if (this->M_PlayerBagBuffer[this->M_FTimeClip_0.M_Count]->M_IsOverlap)
		Grid->M_ItemCountText = FString::FormatAsNumber(this->M_PlayerBagBuffer[this->M_FTimeClip_0.M_Count]->M_Count);
	else
		Grid->M_ItemCountText = FString();

	_Uniform->AddChildToUniformGrid(Grid, (this->M_FTimeClip_0.M_Count / _Col), this->M_FTimeClip_0.M_Count - _Col * (this->M_FTimeClip_0.M_Count / _Col));
}

void UUI_PlayerBag::BagGridButton_BuyGrid(uint8 _Panel_Index, uint8 _Gird_Index, int32 _Price)
{
	FItemPrice Price;
	Price.M_ItemMoneyType = 1;
	Price.M_ItemPrice = _Price;
	Price.M_ItemType = EItemType::E_BagGrid;
	//调出支付界面
	UUI_ShopPayTip* PayPanel = CreateWidget<UUI_ShopPayTip>(this, LoadClass<UUI_ShopPayTip>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/UI_Tip/UI_BuyItem_Tip.UI_BuyItem_Tip_C'")));
	PayPanel->M_ItemDescribe = FString(TEXT("开启格子之后,你可以装备一个背包用于扩充你的存储格子"));
	PayPanel->M_ItemMoneyType = FString(TEXT("礼券"));
	PayPanel->M_ItemPirceText = FString(FString::FormatAsNumber(_Price));
	PayPanel->M_ItemName = FString(TEXT("开启背包格子第") + FString::FormatAsNumber(_Gird_Index) + TEXT("格"));
	PayPanel->M_FItemPrice = Price;
	//关闭输入
	PayPanel->GetEditableTextBox()->SetIsEnabled(false);
	//得到对应界面索引
	PayPanel->GetEditableTextBox()->SetText(FText::FromString(FString::FormatAsNumber(_Panel_Index)));
	PayPanel->AddToViewport();
}


void UUI_PlayerBag::UseCurrentGrid(uint8 _PanelIndex, uint8 _BagNumber, int32 _Price)
{
	TArray<FPlayerBagGird>& Grid = this->LoadBagGridData(_PanelIndex);
	if (Grid.Num() > _BagNumber - 1)
	{
		//当前背包网格名称
		this->M_BagVectorName = FString("PanelGridBag_") + FString::FormatAsNumber(_PanelIndex) + FString::FormatAsNumber(_BagNumber);
		this->M_BagNumber = _BagNumber;

		//从背包中获取已经装备的背包
		TArray<FPlayerBagGird*> BagGrid = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup.GetUseBagArrays();

		FString BagName;

		//从背包组中获取
		for (auto Bag : BagGrid)
		{
			if (Bag->M_VectorName.Equals(this->M_BagVectorName))
			{
				BagName = Bag->M_BagName;
				FPlayerBagGirdGroup::ClearData(*Bag);
			}
		}

		//从背包中寻找背包名称并且取消使用
		for (FEquipmentBase& Euipment : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
		{
			if (Euipment.ItemName.Equals(BagName) && Euipment.M_Used)
			{
				Euipment.M_Used = false;
				if (_PanelIndex == 0) this->LoadItemsData();
				if (_PanelIndex == 1) this->LoadCardData();
				if (_PanelIndex == 2) this->LoadMaterialData();
				return;
			}
		}

		//清除定时器
		UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		//打开背包选择界面
		this->M_PlayerBagContent->ClearChildren();

		//清空背包缓冲数据
		this->M_PlayerBagBuffer.Empty();

		for (FEquipmentBase& _Items_ : UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment)
		{
			//加载背包数据
			if (_Items_.M_EquipmentType == EEquipment::E_Bag && !_Items_.M_Used)
				this->M_PlayerBagBuffer.Emplace(&_Items_);
		}

		this->M_PlayerBagSelectPanel->SetVisibility(ESlateVisibility::Visible);
		this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
			//判断索引是否有效
			if (this->M_FTimeClip_0.M_Count < this->M_PlayerBagBuffer.Num())
			{

				this->LoadEquipmentBagToPlayerBag(this->M_PlayerBagContent, 3);

				this->M_FTimeClip_0.M_Count++;

				//更新背包格子数量显示  如果加载的个数到达最大值，剩余的则不会加载
				if (this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count) == this->M_FTimeClip_0.M_Count)
					UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
			}
			else {
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
			}}, 0.02f, true);

	}
	else {
		//判断前面的格子是否购买
		if (Grid.Num() == _BagNumber - 1)
			//调出支付界面
			this->BagGridButton_BuyGrid(_PanelIndex, _BagNumber, _Price);
	}
}

void UUI_PlayerBag::BagGridButton_1_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 1, 0);
}

void UUI_PlayerBag::BagGridButton_2_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 2, 0);
}

void UUI_PlayerBag::BagGridButton_3_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 3, 0);
}

void UUI_PlayerBag::BagGridButton_4_Func()
{
	this->UseCurrentGrid(this->M_Panel_Index, 4, 0);
}



TArray<FPlayerBagGird>& UUI_PlayerBag::LoadBagGridData(uint8 _PanelNumber)
{
	//判断当前格子是否拥有
	FPlayerBagGirdGroup& Grid = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

	switch (_PanelNumber)
	{
	case 0: return Grid.M_EquipmentGrid;
	case 2: return Grid.M_MaterialGrid;
	}
	return Grid.M_CardGrid;
}

void UUI_PlayerBag::LoadItemsData()
{
	//移除所有界面
	this->Clear();
	this->M_Panel_Index = 0;
	this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FEquipmentBase>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Equipment;
		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < _Items_.Num())
		{
			this->LoadEquipment(_Items_);
			this->M_FTimeClip_0.M_Count++;
			//更新背包格子数量显示  如果加载的个数到达最大值，剩余的则不会加载
			if (this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count) == this->M_FTimeClip_0.M_Count)
			{
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
			}
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		}
		}, 0.02f, true);

}

void UUI_PlayerBag::LoadCardData()
{
	//更新背包格子显示
	this->Clear();
	this->M_Panel_Index = 1;
	this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
		//卡片数据
		TArray<FItemCard>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Card;
		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < _Items_.Num())
		{

			this->LoadCard(_Items_);

			this->M_FTimeClip_0.M_Count++;
			//更新背包格子数量显示  如果加载的个数到达最大值，剩余的则不会加载
			if (this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count) == this->M_FTimeClip_0.M_Count)
			{
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
			}
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		}
		}, 0.02f, true);
}

void UUI_PlayerBag::LoadMaterialData()
{
	//移除所有界面
	this->Clear();
	this->M_Panel_Index = 2;
	this->UpdateBagCount(this->M_Panel_Index, 0);

	this->GetWorld()->GetTimerManager().SetTimer(this->M_FTimeClip_0.M_FTimerHandle, [&]() {
		//材料数据
		TArray<FMaterialBase>& _Items_ = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerItems_Material;
		//判断索引是否有效
		if (this->M_FTimeClip_0.M_Count < _Items_.Num())
		{

			this->LoadMaterial(_Items_);

			this->M_FTimeClip_0.M_Count++;
			//更新背包格子数量显示  如果加载的个数到达最大值，剩余的则不会加载
			if (this->UpdateBagCount(this->M_Panel_Index, this->M_FTimeClip_0.M_Count) == this->M_FTimeClip_0.M_Count)
			{
				UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
			}
		}
		else {
			UGameSystemFunction::ClearTimeClip(this->GetWorld()->GetTimerManager(), this->M_FTimeClip_0);
		}
		}, 0.02f, true);
}

uint8 UUI_PlayerBag::GetPanel_Index()
{
	return this->M_Panel_Index;
}

void UUI_PlayerBag::CloseSelectBagPanel()
{
	this->M_PlayerBagSelectPanel->SetVisibility(ESlateVisibility::Hidden);

	if (this->GetPanel_Index() == 0) this->LoadItemsData();
	if (this->GetPanel_Index() == 1) this->LoadCardData();
	if (this->GetPanel_Index() == 2) this->LoadMaterialData();
}

void UUI_PlayerBag::OpenRemovePanel()
{

	UUI_PlayerBagRemove* PlayerBagRemove = CreateWidget<UUI_PlayerBagRemove>(this, LoadClass<UUI_PlayerBagRemove>(0, TEXT("WidgetBlueprint'/Game/Resource/BP/Game/UI/MainFrame/BP_PlayerBagRemoveItems.BP_PlayerBagRemoveItems_C'")));
	PlayerBagRemove->M_UIBag = this;
	FScriptDelegate CallFunc;

	if (this->GetPanel_Index() == 0)
	{
		PlayerBagRemove->M_PanelNumber = 0;
		CallFunc.BindUFunction(PlayerBagRemove, "SearchBag_Equipment");
		PlayerBagRemove->GetSearchButton()->OnClicked.Add(CallFunc);
	}

	if (this->GetPanel_Index() == 1)
	{
		PlayerBagRemove->M_PanelNumber = 1;
		CallFunc.BindUFunction(PlayerBagRemove, "SearchBag_Card");
		PlayerBagRemove->GetSearchButton()->OnClicked.Add(CallFunc);
	}

	if (this->GetPanel_Index() == 2)
	{
		PlayerBagRemove->M_PanelNumber = 2;
		CallFunc.BindUFunction(PlayerBagRemove, "SearchBag_Materials");
		PlayerBagRemove->GetSearchButton()->OnClicked.Add(CallFunc);
	}

	PlayerBagRemove->AddToViewport();
}

void UUI_PlayerBag::AddBagGrid(int32 _PanelNumber, int32 _BagNumber, TArray<FPlayerBagGird>& _Grid, FEquipmentBase* _Data)
{
	//将当前数据标记为已经使用
	_Data->M_Used = true;

	//加载数据表
	UDataTable* BagData = LoadObject<UDataTable>(0, TEXT("DataTable'/Game/Resource/BP/Data/EquipmentData/EquipmentBagData.EquipmentBagData'"));
	TArray<FTableRowBase*> BagDatas;
	BagData->GetAllRows("List", BagDatas);

	for (auto Items : BagDatas)
	{
		if ((*(FEquipment_Bag_Data*)(Items)).M_FEquipment.ItemName.Equals(_Data->ItemName))
		{
			_Grid[_BagNumber - 1].M_GridCount = (*(FEquipment_Bag_Data*)(Items)).M_FEquipment.M_ContentCount;
			_Grid[_BagNumber - 1].M_GridImagePath = _Data->ItemTexturePath;
			_Grid[_BagNumber - 1].M_PanelNumber = _PanelNumber;
			_Grid[_BagNumber - 1].M_BagNumber = _BagNumber - 1;
			_Grid[_BagNumber - 1].M_VectorName = this->M_BagVectorName;
			_Grid[_BagNumber - 1].M_BagName = _Data->ItemName;
			break;
		}
	}

	this->UpdateBagCount(_PanelNumber, 0);
}

int32 UUI_PlayerBag::UpdateBagCount(uint8 _UpdatePanel, int32 Count)
{
	//FPlayerBagGirdGroup& Bag = UFVMGameInstance::GetFVMGameInstance()->GetPlayerStructManager()->M_PlayerBagGirdGroup;

	//获取玩家购买的背包个数最多4个
	int32 _Count = this->LoadBagGridData(_UpdatePanel).Num();

	//获取背包总容量
	int32 _GridCount = this->SetBagGridStyle(_Count, this->LoadBagGridData(_UpdatePanel));
	//更新文字
	this->M_BagTotalCount_Text_1 = FString(FString::FormatAsNumber(Count) + " / " + FString::FormatAsNumber(_GridCount));
	//返回总容量
	return _GridCount;
}

void UUI_PlayerBag::CloseBagPanel()
{
	this->Clear();
	this->RemoveFromParent();
}
