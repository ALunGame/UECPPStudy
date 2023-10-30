// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

DECLARE_DELEGATE_OneParam(FRegisterRayInfoEvent, TSharedPtr<STextBlock>)

/**
 * 
 */
class SLAICOURSE_API SlAiRayInfoWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiRayInfoWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	
	FRegisterRayInfoEvent RegisterRayInfoEvent;
	

private:

	//获取Menu样式
	const struct FSlAiGameStyle *GameStyle;
	
	//信息
	TSharedPtr<STextBlock> RayInfoTextBlock;

	bool IsInitEvent;
	
};
