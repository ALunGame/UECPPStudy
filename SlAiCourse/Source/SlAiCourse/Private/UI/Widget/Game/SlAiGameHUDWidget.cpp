// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUDWidget.h"

#include "SDPIScaler.h"
#include "SlAiShortcutWidget.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Interactive/SlAiRayInfoWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameHUDWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	//绑定UI缩放规则
	UIScaler.Bind(this,&SlAiGameHUDWidget::GetUIScaler);
	
	ChildSlot
	[
		SNew(SDPIScaler).DPIScale(UIScaler)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ShortcutWidget,SlAiShortcutWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(RayInfoWidget,SlAiRayInfoWidget)
			]
		]
	]; 
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
