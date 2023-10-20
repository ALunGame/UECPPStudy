﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUDWidget.h"

#include "SlAiStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameHUDWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	//绑定UI缩放规则
	UIScaler.Bind(this,&SlAiGameHUDWidget::GetUIScaler);
	
	// ChildSlot
	// [
	// 	
	// ]; 
}

float SlAiGameHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

FVector2d SlAiGameHUDWidget::GetViewportSize() const
{
	FVector2d Result(1920,1080);
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Result);
	}
	return Result;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION