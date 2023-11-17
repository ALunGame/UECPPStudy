// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

struct ChatShowItem;

/**
 * 
 */
class SLAICOURSE_API SlAiChatShowWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiChatShowWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	void AddMessage(FText NewName, FText NewContent);

private:

	void Init();
	
private:
	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	TSharedPtr<SVerticalBox> ChatBox;

	//激活的
	TArray<TSharedPtr<ChatShowItem>> ActiveList;
	
	//激活的
	TArray<TSharedPtr<ChatShowItem>> UnActiveList;
};
