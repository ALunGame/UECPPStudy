// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiHelper.h"

/**
 * 
 */

/**
 * 语言
 */
UENUM()
enum class ECultureTeam : uint8
{
	EN = 0,
	ZH,
};

//Menu按钮的类型
namespace EMenuItem
{
	enum Type : uint8
	{
		None,
		StartGame,
		GameOption,
		QuitGame,
		NewGame,
		LoadRecord,
		
		StartGameGoBack,
		GameOptionGoBack,
		NewGameGoBack,
		ChooseRecordGoBack,

		EnterGame,
		EnterRecord,
	};
}


//Menu界面类型
namespace EMenuType
{
	enum Type : uint8
	{
		None,
		MainMenu,
		StartGame,
		GameOption,
		NewGame,
		ChooseRecord
	};
}


//Menu动画状态
namespace EMenuAnim
{
	enum Type : uint8
	{
		Stop,
		Close,
		Open,
	};
}

/**
 * 视野类型
 */
namespace EViewType
{
	enum Type : uint8
	{
		/**
		* 第一人称
		*/
		First,

		/**
		* 第三人称
		*/
		Third,
	};
}

/**
 * 上半身动画
 */
namespace EUpperBodyAnim
{
	enum Type : uint8
	{
		None,
		Punch,
		Hit,
		Fight,
		PickUp,
		Eat
	};
}


/*
* 物品属性
*/
namespace EObjectType
{
	enum Type : uint8
	{
		Normal = 0,		//普通物品
		Food,		//十五
		Tool,		//工具
		Weapon,		//武器
	};
};

/*
* 物品属性
*/
struct ObjectAttr
{
	FText EN;	//英文名
	FText ZH;	//中文名

	EObjectType::Type ObjectType;

	int PlantAttack;	//对植物的攻击力
	int MetalAttcck;	//对金属的攻击力
	int AnimalAttack;	//对动物的攻击力
	
	int AffectRange;	//攻击力距离

	FString TexPath;	//图片路径

	ObjectAttr(const FText ENName, const FText ZHName, const EObjectType::Type OT, const int PA, const int MA, const int AA, const int AR, const FString TP) {
		EN = ENName;
		ZH = ZHName;
		ObjectType = OT;
		PlantAttack = PA;
		MetalAttcck = MA;
		AnimalAttack = AA;
		AffectRange = AR;
		TexPath = TP;
	}

	FString ToString()
	{
		return EN.ToString() + FString("--") + ZH.ToString() + FString("--") + FString::FromInt((int)ObjectType)
		+ FString("--") + FString::FromInt((int)PlantAttack)
		+ FString("--") + FString::FromInt((int)MetalAttcck)
		+ FString("--") + FString::FromInt((int)AnimalAttack)
		+ FString("--") + FString::FromInt((int)AffectRange)
		+ FString("--") + TexPath;
	}
};

/*
* 快捷栏结构体
*/
struct ShortcutContainer
{
	/*
	* 物品Id
	*/
	int ObjectIndex;
	int ObjectNum;
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;
	const FSlateBrush* NormalContainerBrush;
	const FSlateBrush* ChooseContainerBrush;
	TArray<const FSlateBrush*>* ObjectBrushList;

	ShortcutContainer(TSharedPtr<SBorder> CB, TSharedPtr<SBorder> OI, TSharedPtr<STextBlock> ONT, const FSlateBrush* NCB, const FSlateBrush* CCB, TArray<const FSlateBrush*>* OBL)
	{
		ContainerBorder = CB;
		ObjectImage = OI;
		ObjectNumText = ONT;
		NormalContainerBrush = NCB;
		ChooseContainerBrush = CCB;
		ObjectBrushList = OBL;

		//初始化
		ObjectIndex = 0;
		ObjectNum = 0;
		ContainerBorder->SetBorderImage(NormalContainerBrush);
		ObjectImage->SetBorderImage((*ObjectBrushList)[0]);
	}

	/*
	* 设置选中，返回物品Id
	*/
	int SetChoose(bool Option)
	{
		if (Option)
		{
			ContainerBorder->SetBorderImage(ChooseContainerBrush);
		}
		else
		{
			ContainerBorder->SetBorderImage(NormalContainerBrush);
		}
		return ObjectIndex;
	}

	/*
	* NewIndex
	*/
	ShortcutContainer* SetObject(int NewIndex)
	{
		ObjectIndex = NewIndex;
		ObjectImage->SetBorderImage((*ObjectBrushList)[NewIndex]);
		return this;
	}

	/*
	* 数量
	*/
	ShortcutContainer* SetObjectNum(const int Num = 0)
	{
		ObjectNum = Num;
		if (ObjectNum <= 0)
		{
			ObjectNumText->SetText(FText::FromString(FString("")));
		}
		else
		{
			ObjectNumText->SetText(FText::FromString(FString::FromInt(ObjectNum)));
		}
		return this;
	}
};

/*
* 资源类型
*/
namespace EResourceType {
	enum Type
	{
		Plant = 0,
		Metal,
		Animal
	};
}

/*
* 资源结构体
*/
struct ResourceAttribute 
{
	FText EN;//英文
	FText ZH;//中文
	EResourceType::Type ResourceType;
	int HP;
	TArray<TArray<int>> FlobObjectInfo;

	ResourceAttribute(const FText ENName, const FText ZHName, const EResourceType::Type RT, const int HPValue, TArray<TArray<int>>* FOI) {
		EN = ENName;
		ZH = ZHName;
		ResourceType = RT;
		HP = HPValue;

		//掉落数据
		for (TArray<TArray<int>>::TIterator It(*FOI); It; ++It) {
			TArray<int> FlobObjectInfoItem;
			for (TArray<int>::TIterator Ih(*It); Ih; ++Ih) {
				FlobObjectInfoItem.Add(*Ih);
			}
			FlobObjectInfo.Add(FlobObjectInfoItem);
		}
	}
};

//界面类型
namespace EGameUIType
{
	enum Type
	{
		Game,	//游戏UI
		Pause,	//暂停
		Lose,	//输了，死亡
		Bag,	//背包
		ChatRoom,	//聊天室
	};
}

//背包容器类型
namespace EContainerType
{
	enum Type
	{
		Output,			//合成表输出
		Input,			//合成表输入
		Normal,			//普通容器
		Shortcut,		//快捷栏容器
	};
}

//合成表结构
struct CompoundTable
{
	//合成图
	TArray<int> CompoundMap;

	CompoundTable(TArray<int> *InsertMap) {
		for (TArray<int>::TIterator It(*InsertMap); It; ++It) {
			CompoundMap.Add(*It);
		}
	}

	//获得符合表的输出和数量
	void DetectTable(TArray<int>* IDMap, TArray<int>* NumMap, int& OutputID, int& OutputNum)
	{
		//输出
		int TempID = CompoundMap[9];
		int TempNum = 64;
		for (int i = 0; i < 9; ++i)
		{
			if ((*IDMap)[i] == CompoundMap[i])
			{
				if ((*IDMap)[i] != 0) TempNum = (*NumMap)[i] < TempNum ? (*NumMap)[i] : TempNum;
			}
			else 
			{
				TempID = TempNum = 0;
				break;
			}
		}
		//输出
		if (TempID != 0 && TempNum != 0) {
			OutputID = TempID;
			OutputNum = TempNum;
		}
	}

	//检测合成，并返回消耗表
	bool DetectExpend(TArray<int>* TableMap, int ProductNum, TArray<int>& ExpendMap)
	{
		//检测通过
		bool IsMatch = true;
		for (int i = 0; i < 10; ++i) {
			if ((*TableMap)[i] != CompoundMap[i])
			{
				IsMatch = false;
				break;
			}
		}
		
		//检测通过
		if (IsMatch) {
			for (int i = 0; i < 9; ++i) {
				if (CompoundMap[i] != 0) {
					ExpendMap.Add(ProductNum);
				}
				else {
					ExpendMap.Add(0);
				}
			}
		}
		return IsMatch;
	}

	FString ToString()
	{
		FString OutputString("");
		for (TArray<int>::TIterator It(CompoundMap); It; ++It) {
			OutputString += FString::FromInt(*It) + "-";
		}
		return OutputString;
	}
};

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	ES_Patrol    UMETA(DisplayName = "巡逻"),//巡逻
	ES_Chase     UMETA(DisplayName = "追赶"),//追赶
	ES_Escape    UMETA(DisplayName = "逃跑"),//逃跑
	ES_Attack    UMETA(DisplayName = "攻击"),//攻击
	ES_Hurt      UMETA(DisplayName = "受伤"),//受伤
	ES_Defence   UMETA(DisplayName = "防御"),//防御
	ES_Dead      UMETA(DisplayName = "死亡"),//死亡
};

UENUM(BlueprintType)
enum class EEnemyAttackType : uint8
{
	EA_Normal    UMETA(DisplayName = "普攻"),//普攻
	EA_Dash    UMETA(DisplayName = "Dash"),
	EA_Pursuit    UMETA(DisplayName = "Pursuit"),
};

