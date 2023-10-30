// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactive/SlAiRayInfoWidget.h"

#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiRayInfoWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	IsInitEvent = false;
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(400.f)
		.HeightOverride(100.f)
		[
			SNew(SBorder)
			.BorderImage(&GameStyle->RayInfoBrush)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(RayInfoTextBlock,STextBlock)
				.Font(GameStyle->Font_Outline_50)
				.ColorAndOpacity(GameStyle->FontColor_Black)
			]
		]
	]; 
}

void SlAiRayInfoWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (!IsInitEvent)
	{
		RegisterRayInfoEvent.ExecuteIfBound(RayInfoTextBlock);
		IsInitEvent = true;
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
