// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SlAiShortcutWidget.h"

#include "SlAiDataHandle.h"
#include "SlAiGameMode.h"
#include "SlAiGameWidgetStyle.h"
#include "SlAiPlayerState.h"
#include "SlAiStyle.h"
#include "SlAiTypes.h"
#include "SlateOptMacros.h"
#include "SUniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

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
	WaitFrame = 0;
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
	TArray<TSharedPtr<ShortcutContainer>> ContainerList;
	
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

		TSharedPtr<ShortcutContainer> Container = MakeShareable(new  ShortcutContainer(ContainerBorder,ObjectImg,ObjectNumText,&GameStyle->NormalContainerBrush,
			&GameStyle->ChooseContainerBrush,&SlAiDataHandle::Get()->ObjectBrushList));

		//默认选中
		if (i == 0)
		{
			Container->SetChoose(true);
		}
		
		ContainerList.Add(Container);
	}

	//执行委托
	SlAiHelper::DebugError("SlAiShortcutWidget::InitContainer()>>>>>",60.f);
	RegisterShortcutContainer.ExecuteIfBound(&ContainerList,ShortcutInfoTextBlock);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
