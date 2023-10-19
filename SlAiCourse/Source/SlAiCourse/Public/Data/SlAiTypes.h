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