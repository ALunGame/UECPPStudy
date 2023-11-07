// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "Bag/Container/SlAIContainerBaseWidget.h"

#include "SlAiDataHandle.h"
#include "SlateOptMacros.h"
#include "Bag/Container/SlAiContainerInputWidget.h"
#include "Bag/Container/SlAiContainerNormalWidget.h"
#include "Bag/Container/SlAiContainerOutputWidget.h"
#include "Bag/Container/SlAiContainerShortcutWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAIContainerBaseWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	//位置
	WorkIndex = InArgs._WorkIndex;

	IsHover = false;

	ObjectIndex = 0;

	ObjectNum = 0;
	
	ChildSlot
	[
		SAssignNew(ContainerBorder,SBorder)
		.BorderImage(&GameStyle->NormalContainerBrush)
		.Padding(FMargin(8.f))
		[
			SAssignNew(ObjectImage,SBorder)
			.BorderImage(&GameStyle->EmptyBrush)
			.HAlign(HAlign_Right)
			.VAlign((VAlign_Bottom))
			.Padding(FMargin(0.f,0.f,4.f,0.f))
			[
				SAssignNew(ObjectNumText,STextBlock)
				.Font(GameStyle->Font_Outline_16)
				.ColorAndOpacity(GameStyle->FontColor_Black)
			]
		]
	];
}

TSharedPtr<SlAIContainerBaseWidget> SlAIContainerBaseWidget::CreateContainer(EContainerType::Type NeedType, int WorkID)
{
	TSharedPtr<SlAIContainerBaseWidget> Result;
	switch (NeedType)
	{
	case EContainerType::Output:
		SAssignNew(Result,SlAiContainerOutputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Input:
		SAssignNew(Result,SlAiContainerInputWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Normal:
		SAssignNew(Result,SlAiContainerNormalWidget).WorkIndex(WorkID);
		break;
	case EContainerType::Shortcut:
		SAssignNew(Result,SlAiContainerShortcutWidget).WorkIndex(WorkID);
		break;
	default: ;
	}
	return Result;
}

void SlAIContainerBaseWidget::UpdateHoverd(bool IsHoverd)
{
	if (IsHoverd)
	{
		if (!IsHover)
			ContainerBorder->SetBorderImage(&GameStyle->ChooseContainerBrush);
	}
	else
	{
		if (IsHover)
			ContainerBorder->SetBorderImage(&GameStyle->NormalContainerBrush);
	}
	IsHover = IsHoverd;
}

void SlAIContainerBaseWidget::ResetContainerAttr(int ObjectID, int Num)
{
	if (ObjectIndex != ObjectID) ObjectImage->SetBorderImage(SlAiDataHandle::Get()->ObjectBrushList[ObjectID]);
	ObjectIndex = ObjectID;
	ObjectNum = Num;

	if (ObjectIndex == 0) {
		ObjectNumText->SetText(FText::FromString(""));
	}
	else 
	{
		if (MultiplyAble(ObjectIndex)) {
			ObjectNumText->SetText(FText::FromString(FString::FromInt(ObjectNum)));
		}
		else 
		{
			ObjectNumText->SetText(FText::FromString(""));
		}
	}
}

int SlAIContainerBaseWidget::GetIndex() const
{
	return ObjectIndex;
}

int SlAIContainerBaseWidget::GetNum() const
{
	return ObjectNum;
}

void SlAIContainerBaseWidget::LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//相同可以叠加
	if (InputID == ObjectIndex && MultiplyAble(ObjectIndex)) 
	{
		//鼠标Id
		OutputID = (ObjectNum + InputNum <= 64) ? 0 : InputID;
		//鼠标数量
		OutputNum = (ObjectNum + InputNum <= 64) ? 0 : (ObjectNum + InputNum - 64);

		//设置数量
		ObjectNum = (ObjectNum + InputNum <= 64) ? (ObjectNum + InputNum) : 64;
		ResetContainerAttr(ObjectIndex, ObjectNum);
		return;
	}

	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	ResetContainerAttr(InputID,InputNum);
}

void SlAIContainerBaseWidget::RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum)
{
	//输入为空，自身的一半给出去
	if (InputID == 0)
	{
		OutputID = ObjectIndex;
		OutputNum = (ObjectNum % 2 == 1) ? (ObjectNum / 2 + 1) : (ObjectNum / 2);
		ResetContainerAttr(ObjectNum - OutputNum == 0 ? 0 : ObjectIndex, ObjectNum - OutputNum);
		return;
	}

	//如果物品相同并且可以合并，或者自身为空，添加
	if (ObjectIndex == 0 || (InputID == ObjectIndex && MultiplyAble(InputID)))
	{
		OutputNum = (ObjectNum + 1 <= 64) ? (InputNum - 1) : InputNum;
		OutputID = (OutputNum == 0) ? 0 : InputID;
		ResetContainerAttr(InputID, (ObjectNum + 1 <= 64) ? (ObjectNum + 1) : ObjectNum);
		return;
	}

	//交换
	OutputID = ObjectIndex;
	OutputNum = ObjectNum;
	ResetContainerAttr(InputID, InputNum);
}

bool SlAIContainerBaseWidget::IsEmpty()
{
	return ObjectIndex == 0;
}

bool SlAIContainerBaseWidget::RemainSpace(int ObjetID)
{
	if (ObjectIndex == ObjetID && ObjectNum < 64 && MultiplyAble(ObjectIndex))
	{
		return true;
	}
	return false;
}

bool SlAIContainerBaseWidget::MultiplyAble(int ObjectID)
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	return ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon;
}

bool SlAIContainerBaseWidget::AddObject(int ObjetID)
{
	if (ObjectIndex == 0)
	{
		ResetContainerAttr(ObjetID,1);
		return true;
	}

	if (ObjectIndex == ObjetID && ObjectNum < 64 && MultiplyAble(ObjectIndex))
	{
		ResetContainerAttr(ObjetID,ObjectNum + 1);
		return true;
	}

	return false;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
