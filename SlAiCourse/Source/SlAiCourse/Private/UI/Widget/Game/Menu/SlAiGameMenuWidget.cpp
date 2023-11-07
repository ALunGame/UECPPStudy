﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\..\..\Public\UI\Widget\Game\Menu\SlAiGameMenuWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameMenuWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(300.f)
		.HeightOverride(100.f)
		[
			SNew(STextBlock)
			.Text(FText::FromString("SlAiGameMenuWidget"))
		]
	]; 
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
