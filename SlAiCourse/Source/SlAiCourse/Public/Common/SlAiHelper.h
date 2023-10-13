// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/UnrealEdEngine.h"

namespace SlAiHelper
{
	FORCEINLINE void Debug(FString Message, float Duration = 3.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,Duration,FColor::White,Message);
		}
	}

	FORCEINLINE void DebugWarning(FString Message, float Duration = 3.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,Duration,FColor::Yellow,Message);
		}
	}

	FORCEINLINE void DebugError(FString Message, float Duration = 3.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,Duration,FColor::Red,Message);
		}
	}
}
