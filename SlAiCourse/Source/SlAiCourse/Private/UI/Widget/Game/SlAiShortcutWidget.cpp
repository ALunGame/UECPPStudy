﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SlAiShortcutWidget.h"

#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiShortcutWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(900.f)
		.HeightOverride(160.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(ShortcutInfoTextBlock,STextBlock)
				.Font(GameStyle->Font_Outline_40)
				.ColorAndOpacity(GameStyle->FontColor_White)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(0.f,60.f,0.f,0.f))
			[
				SAssignNew(GridPanel,SUniformGridPanel)
			]
		]
	];

	IsInitContainer = false;
}

void SlAiShortcutWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!IsInitContainer)
	{
		InitContainer();
		IsInitContainer = true;
	}
}

void SlAiShortcutWidget::InitContainer()
{
	for (int i = 0; i < 9; ++i)
	{
		TSharedPtr<SBorder> ContainerBorder;
		TSharedPtr<SBorder> ObjectImg;
		TSharedPtr<STextBlock> ObjectNumText;

		SAssignNew(ContainerBorder,SBorder)
		.Padding(FMargin(10.f))
		.BorderImage(&GameStyle->NormalContainerBrush)
		[
			SAssignNew(ObjectImg,SBorder)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(0.f,0.f,5.f,0.f))
			.BorderImage(&GameStyle->EmptyBrush)
			[
				SAssignNew(ObjectNumText,STextBlock)
				.Font(GameStyle->Font_Outline_20)
				.ColorAndOpacity(GameStyle->FontColor_Black)
				.Text(FText::FromString("12"))
			]
		];

		GridPanel->AddSlot(i,0)
		[
			ContainerBorder->AsShared()
		];
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
