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
