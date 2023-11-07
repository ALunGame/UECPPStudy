// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAIContainerBaseWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiContainerShortcutWidget : public SlAIContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiContainerShortcutWidget)
		{
		}
	SLATE_ATTRIBUTE(int,WorkIndex)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void ResetContainerAttr(int ObjectID, int Num) override;
};
