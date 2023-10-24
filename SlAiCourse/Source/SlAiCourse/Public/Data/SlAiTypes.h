// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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