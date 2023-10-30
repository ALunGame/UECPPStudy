// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "UI/Style/SlAiGameWidgetStyle.h"

/**
 * 
 */
class SLAICOURSE_API SlAiGameHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiGameHUDWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

public:
	
	//DPI缩放系数
	TAttribute<float> UIScaler;
	
	//切换快捷栏
	TSharedPtr<class  SlAiShortcutWidget> ShortcutWidget;

	//射线检测
	TSharedPtr<class  SlAiRayInfoWidget> RayInfoWidget;

private:

	//获取Menu样式
	const struct FSlAiGameStyle *GameStyle;
	
	//绑定UI缩放函数
	float GetUIScaler() const;
	//获得屏幕尺寸
	FVector2d GetViewportSize() const;
};
