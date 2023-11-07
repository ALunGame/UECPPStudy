// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAIContainerBaseWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiContainerNormalWidget : public SlAIContainerBaseWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiContainerNormalWidget)
		{
		}
	SLATE_ATTRIBUTE(int,WorkIndex)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
