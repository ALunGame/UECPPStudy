// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#else
#include "Engine/GameEngine.h"
#endif

/**
 * 
 */
class SLAICOURSE_API SlAiGetter
{
public:
	/**
	 * 获得GameInstance
	 */
	static UGameInstance* GetGameInstance();
};
