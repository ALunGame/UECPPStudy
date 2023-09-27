// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SLAICOURSE_API SlAiStyle
{
public:
	static void Init();

	static FName GetStyleSetName();

	static void ShutDown();

	static const ISlateStyle& Get();

private:
	static TSharedRef<FSlateStyleSet> Create();
	
	static TSharedPtr<FSlateStyleSet> SlAiStyleInstance;
};
