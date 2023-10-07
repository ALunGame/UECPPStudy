// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"

/**
 * 
 */
class SLAICOURSE_API SSlAiMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMenuWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	//绑定到各个按钮点击
	void MenuItemOnClicked(EMenuItem::Type ItemType);

private:
		
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	
	//根节点盒子
	TSharedPtr<SBox> RootSizeBox;

	//标题
	TSharedPtr<STextBlock> TitleText;

	//垂直列表
	TSharedPtr<SVerticalBox> ContentBox;
};
