// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"


DECLARE_DELEGATE_OneParam(FItemClicked, const EMenuItem::Type)

/**
 * 
 */
class SLAICOURSE_API SlAiMenuItemWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiMenuItemWidget)
		{
		}

	
	//属性
	SLATE_ATTRIBUTE(FText,ItemText)
	SLATE_ATTRIBUTE(EMenuItem::Type,ItemType)
	SLATE_EVENT(FItemClicked,OnClicked)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

private:

	FSlateColor GetTintColor() const;

private:

	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	
	//按下事件委托
	FItemClicked OnClicked;

	//类型
	EMenuItem::Type ItemType = EMenuItem::None;

	//按下
	bool IsMouseButtonDown;
};
