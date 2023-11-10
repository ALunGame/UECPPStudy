// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAIEnemyHPWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAIEnemyHPWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:

	void ChangeHP(float HP);

private:

	TSharedPtr<class SProgressBar> HPBar;

	FLinearColor ResultColor;
};
