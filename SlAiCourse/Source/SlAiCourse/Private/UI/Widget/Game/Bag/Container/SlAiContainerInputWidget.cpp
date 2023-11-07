// Fill out your copyright notice in the Description page of Project Settings.


#include "Bag/Container/SlAiContainerInputWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiContainerInputWidget::Construct(const FArguments& InArgs)
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

void SlAiContainerInputWidget::ResetContainerAttr(int ObjectID, int Num)
{
	bool IsChange = false;
	if (ObjectIndex != ObjectID || ObjectNum != Num)
	{
		IsChange = true;
	}

	SlAIContainerBaseWidget::ResetContainerAttr(ObjectID, Num);

	if (IsChange)
	{
		CompoundInput.ExecuteIfBound();
	}
}

void SlAiContainerInputWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	SlAIContainerBaseWidget::LeftOperate(InputID, InputNum, OutputID, OutputNum);
}

void SlAiContainerInputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	SlAIContainerBaseWidget::RightOperate(InputID, InputNum, OutputID, OutputNum);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
