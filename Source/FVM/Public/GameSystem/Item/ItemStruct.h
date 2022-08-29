// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStart/VS/MapBaseType.h"
#include "ItemStruct.generated.h"


USTRUCT(BlueprintType)
struct FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FItemBase() {}
	FItemBase(const FString& _Name, const FString& _IDs, const FString& _TPath) : ItemName(_Name), ItemDescrible(_IDs), ItemTexturePath(_TPath) {}
public:
	//道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName = FString(TEXT("煮蛋器投手"));
	//道具描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescrible = FString(TEXT("可以攻击本行的一个老鼠造成范围伤害,溅射伤害*5%"));
	//显示的纹理路径
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemTexturePath = FString("Texture2D'/Game/Resource/Texture/Sprite/VS/Sprite/Card/CardBg/TCard_0.TCard_0'");
	//是否被认为是等待删除的对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWaitRemove = false;
};


/*

								 装备结构

*/

UENUM(BlueprintType)
enum class EEquipment : uint8
{
	E_Bag UMETA(DisplayName = "Bag"),
	E_Gift UMETA(DisplayName = "Gift"),
	E_PlayerEquipment UMETA(DisplayName = "PlayerEquipment"),
};

//道具->底层
USTRUCT(BlueprintType)
struct FEquipmentBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FEquipmentBase() {};
public:
	//是否被使用
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_Used = false;
	//是否可以道具重叠->如果不能重叠那么一个道具占用一个格子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsOverlap = true;
	//道具数量->基于格子开启了重叠才会生效
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Count = 0;
	//是否是礼盒模式->如果是礼盒模式可以使用【开启功能】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsGift = false;
	//道具类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEquipment M_EquipmentType = EEquipment::E_Bag;
};

//道具->装备背包
USTRUCT(BlueprintType)
struct FBag : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//背包容量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ContentCount = 0;
	//表示背包在序列中的别名->这个是用于判定玩家装备背包的标准【将道具添加到背包之后要判断当前名称是否存在，确保唯一性】
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_VectorName = "";
};

//玩家时装的装备类型
UENUM(BlueprintType)
enum class EPlayerEquipmentType : uint8
{
	//主武器
	E_Weapon UMETA(DisplayName = "Weapon"),
	//副武器->盾牌
	E_SecondWeapon UMETA(DisplayName = "SecondWeapon"),
	//超级武器
	E_SuperWeapon UMETA(DisplayName = "SuperWeapon"),
	//玩家套装(区分套装性别在于后缀的_男_女)
	E_Suit UMETA(DisplayName = "Suit"),
};

//道具->玩家装备(时装)
USTRUCT(BlueprintType)
struct FPlayerEquipment : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//装备类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerEquipmentType M_PlayerEquipmentType = EPlayerEquipmentType::E_Suit;
};


UENUM(BlueprintType)
enum class EGiftBox_Item_Type : uint8
{
	E_Card UMETA(DisplayName = "Card"),
	E_Equipment UMETA(DisplayName = "Equipment"),
	E_Material UMETA(DisplayName = "Material"),
	E_Coin UMETA(DisplayName = "Coin"),
};

//礼盒具体奖励设置
USTRUCT(BlueprintType)
struct FGiftBox_Item {
	GENERATED_USTRUCT_BODY()
public:
	//物品名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ItemName = FString();
	//物品类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGiftBox_Item_Type M_ItemType = EGiftBox_Item_Type::E_Card;
	//选择当前类型全部(为true时,当前类型的道具全部都会获取,但是不包括礼盒)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SelectCurrentTypeAll = false;
	//货币类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CoinType = 0;
	//卡片等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardGrade = 0;
	//物品的数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ItemCount = 1;
	//礼盒数字(该数字只会被礼盒随机模式影响)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GiftNumber = 1;
};

//道具->礼盒
USTRUCT(BlueprintType)
struct FGiftBox : public FEquipmentBase {
	GENERATED_USTRUCT_BODY()
public:
	//开启随机模式
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_bEnableRandomMode = false;
	//最大随机数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_RandomMaxNumber = 100;
	//最大获取个数(小于等于0则表示全部获取大于最大个数则全部获取)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_GetGiftMaxCount = 1;
	//礼盒奖励内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FGiftBox_Item> M_GiftContent;
};

/*

								 卡片结构

*/

//-----------------------------------------   卡片类型->拥有这个类型表示属于那一类

UENUM(BlueprintType)
enum class ECardType : uint8 {
	//攻击型
	E_ATK UMETA(DisplayName = "ATK_Type"),
	//生产型
	E_SPAWN UMETA(DisplayName = "Spawn_Type"),
	//防御型
	E_DEFENCE UMETA(DisplayName = "Defence_Type"),
	//辅助型
	E_Aid UMETA(DisplayName = "Aid_Type"),
	//功能型
	E_Function UMETA(DisplayName = "Function_Type"),

};

//-----------------------------------------   卡片技能升级类型->拥有这个类型表示会得到这个类型的效果
UENUM(BlueprintType)
enum class ECardSkillType : uint8 {
	//无
	E_NULL UMETA(DisplayName = "NULL_Type"),
	//生产量提升
	E_SpawnFlameNumUp UMETA(DisplayName = "SpawnFlameNumUp_Type"),
	//冷却速度提升
	E_COLDDOWN UMETA(DisplayName = "ColdDown_Type"),
	//攻击速度提升
	E_ATTACKSPEED UMETA(DisplayName = "AttackSpeed_Type"),
};

//-----------------------------------------   卡片星星升级类型->表示每上升一个等级会随之提升某一项数据
UENUM(BlueprintType)
enum class ECardUpGradeType : uint8 {
	//攻击力提升
	E_Atk_Up UMETA(DisplayName = "Atk_Up"),
	//生产速度提升
	E_SpawnSpeed_Up UMETA(DisplayName = "SpawnSpeed_Up"),
	//冷却速度提升
	E_ColdDownSpeed_Up UMETA(DisplayName = "ColdDownSpeed_Up"),
	//防御力(血量)提升
	E_Defence_Up UMETA(DisplayName = "Defence_Up"),
};


//卡片转职
USTRUCT(BlueprintType)
struct FCardChangeJobs {
	GENERATED_USTRUCT_BODY()
public:
	//是否可以转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_IsChange = false;
	//转职对象
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_ChangeMaterialsName = "";
	//转职类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardType M_ChangeType = ECardType::E_ATK;
	//转职材料
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> M_ChangeMaterials = {};
	//转职等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ChangeGrade = 9;
};

//等级提升结构
USTRUCT(BlueprintType)
struct FGrade {
	GENERATED_USTRUCT_BODY()
public:
	//当前经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentEx = 0.f;
	//当前经验值最大值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CurrentTopEx = 100.f;
	//下一次经验值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_NextEx = 150.f;
	//经验值成长率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_NextExUpRate = 0.2f;
	//当前等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CurrentGrade = 0;
	//最大等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MaxGrade = 10;
};

//卡片基础底层
USTRUCT(BlueprintType)
struct FItemCard : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	FItemCard() {}
public:
	//卡片属于白天还是夜晚
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardDay = true;
	//卡片价格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardPrice = 250;
	//冷却时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardColdDown = 3.f;
	//卡片星星等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardGrade = 0;
	//卡片生命值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardHP = 10.f;
	//卡片优先级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardLayer = 0;
	//卡片是否可以重叠(覆盖)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardOverlap = false;
	//卡片重叠对象名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_CardOverlapName;
	//卡片价格是否随玩家放置个数线性增加
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_CardPriceAutoUp = false;
	//卡片每次增加25火苗
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_CardPriceUpNum = 25;
	//卡片转职
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCardChangeJobs M_FCardChangeJobs;
	//卡片类型(用于结构转型)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardType M_ECardType = ECardType::E_ATK;
	//卡片技能升级后开启的能力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardSkillType M_ECardSkillType = ECardSkillType::E_ATTACKSPEED;
	//卡片星星升级类型->表示每上升一个等级会随之提升某一项数据
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardUpGradeType M_ECardUpGradeType = ECardUpGradeType::E_Atk_Up;
	//因为等级每次的提升比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_M_ECardUpGradeUpRate = 0.f;
	//卡片线路类型->顾名思义就是种植的当前网格，如果网格是【陆地】类型（当前卡片类型必须是:陆地）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineType M_ELineType = ELineType::OnGround;
	//道具描述2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescrible_2 = "";
	//卡片对应的蓝图实例(Card_0表示煮蛋器投手)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemTarget_ActorFileName = "";
};


//-----------------------------------------   卡片类型


//射线位置
UENUM(BlueprintType)
enum class ELineTracePosition : uint8 {
	E_Left  UMETA(DisplayName = "Left"),
	E_Right UMETA(DisplayName = "Right"),
	E_LeftRight_M UMETA(DisplayName = "LeftRight_M"),
	E_Top UMETA(DisplayName = "Top"),
	E_Bottom UMETA(DisplayName = "Bottom"),
	E_TopBottom_M UMETA(DisplayName = "TopBottom_M")
};

//射线偏移结构设置
USTRUCT(BlueprintType)
struct FLineTraceSetting {
	GENERATED_USTRUCT_BODY()
public:
	//开始位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_BeginLocation = FVector();
	//结束位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector M_EndLocation = FVector();
	//设置位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELineTracePosition M_ELineTracePosition = ELineTracePosition::E_Right;
};

//攻击类型防御卡
USTRUCT(BlueprintType)
struct FItemCardATK : public FItemCard {
	GENERATED_USTRUCT_BODY()
public:
	//卡片基础攻击力
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKBase = 0.f;
	//卡片攻击力加成比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_CardATKRate = 0.f;
	//是否有溅射伤害
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_SputteringHurt = false;
	//溅射伤害比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SputteringATKRate = 0.f;
	//攻击速度(攻击延迟)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootSpeed = 2.f;
	//首次投射时间 {延迟}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_FirstShootSpeed = 0.5f;
	//投射延迟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ShootDelay = 0.1f;
	//投射多少发
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_ProjectionItemCount = 1;
	//射线设置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FLineTraceSetting> M_LineTraceSettings;
};

//生产类型防御卡
USTRUCT(BlueprintType)
struct FItemCardSpawn : public FItemCard {
	GENERATED_USTRUCT_BODY()
public:
	//每一个火苗初始量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_FlameNum = 25;
	//一次生产几个火苗
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_SpawnNum = 1;
	//生产速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnSpeed = 15.f;
	//生产速度提升比率
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnSpeedRate = 0.034f;
	//是否开启周期生长
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_EnableWeekUp = false;
	//生长的时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_WeekUpTime = 60.f;
	//生产火苗数量提升比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_SpawnFlameNumRate = 2.f;
};

//防御类型防御卡
USTRUCT(BlueprintType)
struct FItemCardDefence : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:
	//防御力加成比例(基于生命值加成)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_DefenceUpRate = 0.034f;
	//反伤
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK = 0.f;
	//反伤加成比例
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_ATK_UpRate = 0.f;;
};

//辅助类型防御卡
USTRUCT(BlueprintType)
struct FItemCardAid : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:

};

//功能类型防御卡
USTRUCT(BlueprintType)
struct FItemCardFunction : public FItemCard {
	GENERATED_USTRUCT_BODY();
public:
	//黑夜转换白天状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_TransfromNigntToDay = false;
	//白天转换黑天状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_TransfromDayToNignt = false;
	//刷新当前卡片冷却
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_RefreshCurrentCardClodDown = false;
	//使用上一次放置的卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_UseBackCard = false;
	//随机放置一张卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool M_RandomPlayCard = false;
};
/*

								 材料结构

*/

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	//卡片合成配方
	E_Blueprint UMETA(DisplayName = "Blueprint"),
	//卡片配方材料
	E_CardSynthesisMaterial UMETA(DisplayName = "CardSynthesisMaterial"),
	//卡片转职材料
	E_CardChangeJobMaterial UMETA(DisplayName = "CardChangeJobMaterial"),
	//香料
	E_Spices UMETA(DisplayName = "Spices"),
	//四叶草
	E_Clover UMETA(DisplayName = "Clover"),
};

//道具->材料->底层
USTRUCT(BlueprintType)
struct FMaterialBase : public FItemBase {
	GENERATED_USTRUCT_BODY()
public:
	//数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_Count = 0;
	//材料类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMaterialType M_MaterialType = EMaterialType::E_Blueprint;
};

//材料->卡片配方
USTRUCT(BlueprintType)
struct FCardBlueprint : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//需要的材料道具名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> M_Materials = {};
	//合成的卡片类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardType M_SynthesisType = ECardType::E_ATK;
	//合成的卡片名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_SynthesisCardName = "";
};

//材料->卡片配方材料
USTRUCT(BlueprintType)
struct FCardBlueprintMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	FCardBlueprintMaterial() {}
};


//卡片转职序号
UENUM(BlueprintType)
enum class ECardChangeType : uint8
{
	E_A UMETA(DisplayName = "A"),
	E_B UMETA(DisplayName = "B"),
	E_C UMETA(DisplayName = "C"),
};

//材料->卡片转职材料
USTRUCT(BlueprintType)
struct FCardChangeJobMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//转职目标卡片
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString M_TargetCardName = "";
	//转职的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECardChangeType M_CardChangeType = ECardChangeType::E_A;
};

//材料->香料
USTRUCT(BlueprintType)
struct FSpicesMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//可以制作的卡片等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 M_MakeCardGrade = 1;
};

//材料->四叶草
USTRUCT(BlueprintType)
struct FCloverMaterial : public FMaterialBase {
	GENERATED_USTRUCT_BODY()
public:
	//可以帮助概率提升x倍
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float M_UpGrateRate = 1.35f;
};



UCLASS()
class FVM_API UItemStruct : public UObject
{
	GENERATED_BODY()
};
