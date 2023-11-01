// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiPointerWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiPointerWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	//更新准心
	void UpdatePointer(bool IsAnim,float Range);

private:

	//大小
	FOptionalSize GetBoxSize() const;
	
private:
	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	TSharedPtr<class SBox> RootBox;

	float CurrSize;

	//动态材质指针
	class UMaterialInstanceDynamic* PointerMaterial;

	//是否改变大小
	bool IsAnimed;
};
