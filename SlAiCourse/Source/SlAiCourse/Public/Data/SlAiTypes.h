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