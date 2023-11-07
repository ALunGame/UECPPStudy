// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAIContainerBaseWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiContainerOutputWidget : public SlAIContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiContainerOutputWidget)
		{
		}
	SLATE_ATTRIBUTE(int,WorkIndex)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;

	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum) override;
};
