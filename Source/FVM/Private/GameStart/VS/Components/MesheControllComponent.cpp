// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStart/VS/Components/MesheControllComponent.h"
#include "GameSystem/GameMapStructManager.h"
#include "GameStart/VS/MapMeshe.h"


// Sets default values for this component's properties
UMesheControllComponent::UMesheControllComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


UGameMapStructManager* UMesheControllComponent::SetGameMapStructManager(UGameMapStructManager* _GameMapStructManager)
{
	if (!_GameMapStructManager)
		return nullptr;

	this->M_GameMapStructManager = _GameMapStructManager;

	this->InitGameMapMeshe();

	return _GameMapStructManager;
}

void UMesheControllComponent::ClearAllMeshes()
{
	//清空所有Actor
	for (auto Interator = this->M_Meshes.CreateIterator(); Interator; Interator++)
	{
		(*Interator)->Destroy();
	}
	this->M_Meshes.Empty();
}

void UMesheControllComponent::SpawnMeshes()
{
	//超过行最大个数
	if ((uint8)(this->M_GameMapStructManager->M_Data.M_RowMesheCount) > (uint8)(this->M_GameMapStructManager->M_Data.M_MesheRowMax))
	{
		return;
	}

	//超过列最大个数
	if ((uint8)(this->M_GameMapStructManager->M_Data.M_ColMesheCount) > (uint8)(this->M_GameMapStructManager->M_Data.M_MesheColMax))
	{
		return;
	}

	//生成第一个网格位置
	const FVector2D& FristMeshPosition = this->M_GameMapStructManager->M_Data.M_FirstRowMesheLocation;

	//网格位置
	FTransform CurrentMesheLocation;
	CurrentMesheLocation.SetRotation(FRotator(0.f, 180.f, 0.f).Quaternion());
	CurrentMesheLocation.SetScale3D(FVector(1.f, 1.f, 1.f));

	//初始化默认行间距
	if (this->M_GameMapStructManager->M_Data.M_RowMargin.Num() != this->M_GameMapStructManager->M_Data.M_RowMesheCount)
		this->M_GameMapStructManager->M_Data.M_RowMargin.Init(this->M_GameMapStructManager->M_Data.M_DefRowUniformMargin, this->M_GameMapStructManager->M_Data.M_RowMesheCount);

	//生成Actor对象
	for (int32 Row = 0, RowMargin = 0; Row < this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount; Row++)
	{
		//层级
		float Leyer = 0.f;
		for (int32 Col = 0; Col < this->M_GameMapStructManager->M_Data.M_EnableColMesheCount; Col++)
		{
			//设置位置
			CurrentMesheLocation.SetLocation(FVector((550.f - Leyer) - Row, FristMeshPosition.X + Col * this->M_GameMapStructManager->M_Data.M_ColMargin, FristMeshPosition.Y - RowMargin));
			//得到地图网格
			AMapMeshe* M_TempMapMesh = this->GetWorld()->SpawnActor<AMapMeshe>(this->M_AMapMeshe, CurrentMesheLocation);
			//获得网格的索引
			int32 _GrideIndex = Row * this->M_GameMapStructManager->M_Data.M_EnableColMesheCount + Col;
			//设置网格类型
			if (this->M_GameMapStructManager->M_Data.M_GradeLineType.Num() > _GrideIndex)
				M_TempMapMesh->SetLine(Row, Col, this->M_GameMapStructManager->M_Data.M_GradeLineType[_GrideIndex]);
			else
				M_TempMapMesh->SetLine(Row, Col, ELineType::OnGround);

			UE_LOG(LogTemp, Error, TEXT("%d行,%d列,%d格::%d"), Row, Col, _GrideIndex, (uint8)M_TempMapMesh->M_ELineType);

			//设置位置变化
			M_TempMapMesh->SetWidgetComponentTransform(CurrentMesheLocation);
			//添加Actor到列表
			this->M_Meshes.Emplace(M_TempMapMesh);
			//每一列的层级会不断递增
			Leyer += 0.1f;
		}
		//更新Margin
		RowMargin += this->M_GameMapStructManager->M_Data.M_RowMargin[Row];
		UE_LOG(LogTemp, Error, TEXT("---"));
	}
}

void UMesheControllComponent::UpdateMeshes()
{
	//如果启用的行大于生成的行
	if (this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount > this->M_GameMapStructManager->M_Data.M_RowMesheCount)
	{
		return;
	}

	//如果启用的列大于生成的列
	if (this->M_GameMapStructManager->M_Data.M_EnableColMesheCount > this->M_GameMapStructManager->M_Data.M_ColMesheCount)
	{
		return;
	}

	//清空坐标
	this->M_LineLastLocation.Empty();

	//开启Actor设置
	for (int32 Row = 0; Row < this->M_GameMapStructManager->M_Data.M_RowMesheCount; Row++)
	{
		for (int32 Col = 0; Col < this->M_GameMapStructManager->M_Data.M_ColMesheCount; Col++)
		{
			//如果满足条件启用Actor  (Row小于启用的个数) (Col小于启用的个数)
			if (Row < this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount && Col < this->M_GameMapStructManager->M_Data.M_EnableColMesheCount)
			{
				this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_ColMesheCount) + Col]->SetActorHiddenInGame(false);
				this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_ColMesheCount) + Col]->SetActorTickEnabled(true);
				//UE_LOG(LogTemp, Warning, TEXT("%d"), (Row * this->M_GameMapStructManager->M_Data.M_ColMesheCount));
			}
			else {
				this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_ColMesheCount) + Col]->SetActorHiddenInGame(true);
				this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_ColMesheCount) + Col]->SetActorTickEnabled(false);
			}
		}
	}

	//更新坐标
	for (int32 Row = 0; Row < this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount; Row++)
	{
		this->M_LineLastLocation.Emplace(this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_EnableColMesheCount) + this->M_GameMapStructManager->M_Data.M_EnableColMesheCount - 1]->GetActorLocation());
	}
}
 
AMapMeshe* UMesheControllComponent::GetMapMesh(int32 _Row, int32 _Col)
{
	return Cast<AMapMeshe>(this->M_Meshes[(_Row * this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount) + _Col]);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentTop(int32 _Col)
{
	return Cast<AMapMeshe>(this->M_Meshes[_Col]);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentBottom(int32 _Col)
{
	return Cast<AMapMeshe>(this->M_Meshes[((this->M_GameMapStructManager->M_Data.M_EnableRowMesheCount - 1) * this->M_GameMapStructManager->M_Data.M_EnableColMesheCount) + _Col]);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentLeft(int32 Row)
{
	return Cast<AMapMeshe>(this->M_Meshes[Row * this->M_GameMapStructManager->M_Data.M_EnableColMesheCount]);
}

AMapMeshe* UMesheControllComponent::GetMapMeshCurrentRight(int32 Row)
{
	return Cast<AMapMeshe>(this->M_Meshes[(Row * this->M_GameMapStructManager->M_Data.M_EnableColMesheCount) + this->M_GameMapStructManager->M_Data.M_EnableColMesheCount - 1]);
}

// Called when the game starts
void UMesheControllComponent::BeginPlay()
{
	Super::BeginPlay();

	//加载Actor
	this->M_AMapMeshe = LoadClass<AMapMeshe>(0, TEXT("Blueprint'/Game/Resource/BP/GameStart/VS/AMapMeshe.AMapMeshe_C'"));

	//如果加载失败则返回
	if (!this->M_AMapMeshe)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]M_AMapMeshe is a nullptr"), __FUNCTION__);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *FString(__FUNCTION__ + FString("M_AMapMeshe is a nullptr")));
	}
}

void UMesheControllComponent::InitGameMapMeshe()
{
	this->ClearAllMeshes();
	this->SpawnMeshes();
	this->UpdateMeshes();
}


// Called every frame
void UMesheControllComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

