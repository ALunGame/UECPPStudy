// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SSlAiMenuHUDWidget.h"

#include "SDPIScaler.h"
#include "SlateOptMacros.h"
#include "Style/SlAiMenuWidgetStyle.h"
#include "Style/SlAiStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiMenuHUDWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//绑定UI缩放规则
	UIScaler.Bind(this,&SSlAiMenuHUDWidget::GetUIScaler);
	
	ChildSlot
	[
		SNew(SDPIScaler).DPIScale(UIScaler)
		[
			SNew(SOverlay)

			//背景图片
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuHUDBackgroundBrush)
			]

			//菜单图片
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuBackgroundBrush)
			]
		]
	]; 
}

float SSlAiMenuHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

FVector2d SSlAiMenuHUDWidget::GetViewportSize() const
{
	FVector2d Result(1920,1080);
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Result);
	}
	return Result;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
