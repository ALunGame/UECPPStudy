// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiChooseRecordWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiChooseRecordWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void UpdateRecordName();

private:
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	
	//输入框
	TSharedPtr<STextComboBox> RecordComboBox;
	TArray<TSharedPtr<FString>> OptionSource;
};
