﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiShortcutWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiShortcutWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
