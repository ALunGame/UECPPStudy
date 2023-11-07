// Fill out your copyright notice in the Description page of Project Settings.


#include "Bag/Container/SlAiContainerOutputWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiContainerOutputWidget::Construct(const FArguments& InArgs)
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

void SlAiContainerOutputWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//自身为空，不操作
	if (ObjectIndex == 0)
	{
		OutputID = InputNum;
		OutputNum = InputNum;
		return;
	}

	//输入物品与自身相同且可以叠加
	if (InputID == ObjectIndex && MultiplyAble(InputID))
	{
		OutputID = ObjectIndex;
		OutputNum = (InputNum + ObjectNum <= 64) ? (InputNum + ObjectNum) : 64;

		ObjectNum = (InputNum + ObjectNum <= 64) ? 0 : (InputNum + ObjectNum - 64);

		//合成
		CompoundOutput.ExecuteIfBound(ObjectIndex,OutputNum - InputNum);
		return;
	}

	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	CompoundOutput.ExecuteIfBound(ObjectIndex,ObjectNum);
	ThrowObject.ExecuteIfBound(InputID,InputNum);
}

void SlAiContainerOutputWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//自身为空，不操作
	if (ObjectIndex == 0)
	{
		OutputID = InputNum;
		OutputNum = InputNum;
		return;
	}

	//输入空，取一半
	if (InputID == 0)
	{
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum % 2 == 1) ? (ObjectNum / 2 + 1) : (ObjectNum / 2);
		//合成
		CompoundOutput.ExecuteIfBound(ObjectIndex,OutputNum);
		return;
	}

	//相同并且可以合并
	if (InputID == ObjectIndex && MultiplyAble(InputID)) {
		OutputID = ObjectIndex;
		int PreOutputNum = (ObjectNum % 2 == 1) ? (ObjectNum / 2 + 1) : (ObjectNum / 2);
		OutputNum = (PreOutputNum + InputNum <= 64) ? (PreOutputNum + InputNum) : 64;
		ObjectNum = ObjectNum - (OutputNum - InputNum);
		CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum - InputNum);
		return;
	}

	//输入丢弃，输出一半
	OutputID = ObjectIndex;
	OutputNum = (ObjectNum % 2 == 1) ? (ObjectNum / 2 + 1) : (ObjectNum / 2);
	CompoundOutput.ExecuteIfBound(ObjectIndex, OutputNum);
	ThrowObject.ExecuteIfBound(InputID, InputNum);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
