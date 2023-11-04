// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiPlayerStateWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiPlayerStateWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//更新状态事件
	void UpdateStateWidget(float HPValue, float HungerValue);

private:
	//获取Menu样式
	const struct FSlAiGameStyle *GameStyle;
	
	class UMaterialInstanceDynamic* HPMaterial;
	class UMaterialInstanceDynamic* HungerMaterial;
};
