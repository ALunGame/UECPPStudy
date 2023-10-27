// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"

//注册容器到PlayerState类的委托
DECLARE_DELEGATE_TwoParams(FRegisterShortcutContainer, TArray<TSharedPtr<ShortcutContainer>>*, TSharedPtr<STextBlock>)

/**
 * 
 */
class SLAICOURSE_API SlAiShortcutWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiShortcutWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	FRegisterShortcutContainer RegisterShortcutContainer;
	

//private:

	//初始化容器
	void InitContainer();

	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainers;
	
//private:

	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	//信息
	TSharedPtr<class STextBlock> ShortcutInfoTextBlock;

	//网格
	TSharedPtr<class SUniformGridPanel> GridPanel;

	bool IsInitContainer;

	int WaitFrame;
};
