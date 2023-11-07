// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SUniformGridPanel.h"

/**
 * 
 */
class SLAICOURSE_API SlAiBagWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiBagWidget)
		{
		}

	SLATE_ATTRIBUTE(float,UIScaler)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//注册背包管理器事件
	void InitBagManger();

protected:
	
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	//重写绘制函数
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	//鼠标点击
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
private:

	//获取Menu样式
    const struct FSlAiGameStyle *GameStyle;

	//初始化
	bool IsInitBagManager;

	//快捷栏表格
	TSharedPtr<SUniformGridPanel> ShortcutGrid;

	//背包表格
	TSharedPtr<SUniformGridPanel> BagGrid;

	//合成表格
	TSharedPtr<SUniformGridPanel> CompoundGrid;

	//输出容器
	TSharedPtr<SBorder> OutputBorder;

	//鼠标位置
	FVector2d MousePosition;

	//DPI缩放
	TAttribute<float> UIScaler;
};
