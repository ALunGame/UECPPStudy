// Fill out your copyright notice in the Description page of Project Settings.


#include "Bag/Container/SlAiContainerNormalWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiContainerNormalWidget::Construct(const FArguments& InArgs)
{
	SlAIContainerBaseWidget::Construct(
	SlAIContainerBaseWidget::FArguments().WorkIndex(InArgs._WorkIndex)
	);
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
