// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SScrollBox.h"

struct ChatMessItem;

DECLARE_DELEGATE_TwoParams(FPushMessage, FText, FText)

/**
 * 
 */
class SLAICOURSE_API SlAiChatRoomWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiChatRoomWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType);

	FReply SendEvent();

	void AddMessage(FText NewName, FText NewContent);

	void ScrollToEnd();

public:

	FPushMessage PushMessage;

private:
	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	TSharedPtr<SScrollBox> ScrollBox;
	
	TSharedPtr<SEditableTextBox> EditableTextBox;

	TArray<TSharedPtr<ChatMessItem>> MessageList;
};
