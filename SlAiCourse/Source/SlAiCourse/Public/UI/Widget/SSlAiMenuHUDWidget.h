// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSlAiMenuWidget.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SSlAiMenuHUDWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMenuHUDWidget)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	//绑定UI缩放函数
	float GetUIScaler() const;
	//获得屏幕尺寸
	FVector2d GetViewportSize() const;

private:
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;

	//DPI缩放系数
	TAttribute<float> UIScaler;

	//菜单指针
	TSharedPtr<SSlAiMenuWidget> MenuWidget;
};
