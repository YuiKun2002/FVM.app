// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MesheControllComponent.generated.h"

class UGameMapStructManager;
class AMapMeshe;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FVM_API UMesheControllComponent : public UActorComponent
{
	GENERATED_BODY() 
private:
	//地图配置管理器
	UPROPERTY(EditAnywhere)
		UGameMapStructManager* M_GameMapStructManager = nullptr;
	//地图的网格
	UPROPERTY(EditAnywhere)
		UClass* M_AMapMeshe = nullptr;
	//所有创建的网格集合
	UPROPERTY(EditAnywhere)
		TArray<AActor*> M_Meshes;
public:
	//每一行的最后一个的位置
	UPROPERTY(EditAnywhere)
		TArray<FVector> M_LineLastLocation;
public:
	// Sets default values for this component's properties
	UMesheControllComponent();
	//设置游戏地图配置
	UFUNCTION(BlueprintCallable)
		UGameMapStructManager* SetGameMapStructManager(UGameMapStructManager* _GameMapStructManager);
	//删除所有的网格
	UFUNCTION(BlueprintCallable)
		void ClearAllMeshes();
	//生成网格
	UFUNCTION(BlueprintCallable)
		void SpawnMeshes();
	//更新网格
	UFUNCTION(BlueprintCallable)
		void UpdateMeshes();

	//获取网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMesh(int32 _Row, int32 _Col);
	//获取当前网格的最顶部网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMeshCurrentTop(int32 _Col);
	//获取当前网格的最底部网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMeshCurrentBottom(int32 _Col);
	//获取当前网格的最左边的网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMeshCurrentLeft(int32 Row);
	//获取当前网格的最右边的网格
	UFUNCTION(BlueprintCallable)
		AMapMeshe* GetMapMeshCurrentRight(int32 Row);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//初始化地图
	void InitGameMapMeshe();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};
