// Fill out your copyright notice in the Description page of Project Settings.


#include "Bag/Container/SlAiContainerShortcutWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiContainerShortcutWidget::Construct(const FArguments& InArgs)
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

void SlAiContainerShortcutWidget::ResetContainerAttr(int ObjectID, int Num)
{
	bool IsChange = false;
	if (ObjectIndex != ObjectID || ObjectNum != Num)
	{
		IsChange = true;
	}

	SlAIContainerBaseWidget::ResetContainerAttr(ObjectID, Num);

	if (IsChange)
	{
		BagShortChange.ExecuteIfBound(WorkIndex.Get(),ObjectID, Num);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
