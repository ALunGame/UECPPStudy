// Fill out your copyright notice in the Description page of Project Settings.
#include "SlAiChatRoomWidget.h"

#include "SlAiStyle.h"
#include "SlAiGameWidgetStyle.h"
#include "SlateOptMacros.h"
#include "SMultiLineEditableText.h"

struct ChatMessItem
{
	//水平组件
	TSharedPtr<SHorizontalBox> CSBox;
	//名字
	TSharedPtr<STextBlock> CSName;
	//内容框
	TSharedPtr<SBorder> CSBorder;
	//内容
	TSharedPtr<SMultiLineEditableText> CSContent;

	ChatMessItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		SAssignNew(CSBox, SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		[
			SAssignNew(CSName, STextBlock)
			.Font(FontInfo)
			.ColorAndOpacity(TAttribute<FSlateColor>(FSlateColor(FLinearColor(0.f, 1.f, 0.f, 1.f))))
		]

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillWidth(1.f)
		[
			SAssignNew(CSBorder, SBorder)
			.BorderImage(EmptyBrush)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(CSContent, SMultiLineEditableText)
				.WrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping)
				.AutoWrapText(true)
				.Font(FontInfo)
			]
		];
	}
	
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent) {
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		return CSBox;
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiChatRoomWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[

		SNew(SBox)
		.WidthOverride(600.f)
		.HeightOverride(1080.f)
		[
			SNew(SBorder)
			.BorderImage(&GameStyle->ChatRoomBGBrush)
			[
				SNew(SOverlay)

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Bottom)
				.Padding(FMargin(0.f, 0.f, 0.f, 80.f))
				[
					SAssignNew(ScrollBox, SScrollBox)
				]

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(0.f, 1000.f, 0.f, 0.f))
				[
			
					SNew(SOverlay)
			
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(0.f, 0.f, 120.f, 0.f))
					[
						SAssignNew(EditableTextBox, SEditableTextBox)
						.Font(GameStyle->Font_30)
						.OnTextCommitted(this, &SlAiChatRoomWidget::SubmitEvent)
					]

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Padding(FMargin(480.f, 0.f, 0.f, 0.f))
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.OnClicked(this, &SlAiChatRoomWidget::SendEvent)
						[
							SNew(STextBlock)
							.Font(GameStyle->Font_30)
							.Text(NSLOCTEXT("SlAiGame", "Send", "Send"))
						]
					]
				]
			]
		]
	];
	
	AddMessage( FText::FromString("aaaa"),FText::FromString("acccccaaa"));
}

void SlAiChatRoomWidget::SubmitEvent(const FText& SubmitText, ETextCommit::Type CommitType)
{
	FString MessageStr = EditableTextBox->GetText().ToString();
	if (MessageStr.IsEmpty())
		return;

	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	EditableTextBox->SetText(FText::FromString(""));
	PushMessage.ExecuteIfBound(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr)); 
}

FReply SlAiChatRoomWidget::SendEvent()
{
	FString MessageStr = EditableTextBox->GetText().ToString();
	if (MessageStr.IsEmpty())
		return FReply::Handled();
	
	MessageStr = FString(": ") + MessageStr;
	AddMessage(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	EditableTextBox->SetText(FText::FromString(""));
	
	PushMessage.ExecuteIfBound(NSLOCTEXT("SlAiGame", "Player", "Player"), FText::FromString(MessageStr));
	return FReply::Handled();
}

void SlAiChatRoomWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatMessItem> InsertItem;

	//最大三十
	if (MessageList.Num() < 30)
	{
		//新建
		InsertItem = MakeShareable(new ChatMessItem(&GameStyle->EmptyBrush, GameStyle->Font_30));
		MessageList.Add(InsertItem);
		ScrollBox->AddSlot()
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
	}
	else
	{
		//拿出第一个
		InsertItem = MessageList[0];
		MessageList.Remove(InsertItem);
		ScrollBox->RemoveSlot(InsertItem->CSBox->AsShared());
		//复用在最后
		ScrollBox->AddSlot()
		[
			InsertItem->ActiveItem(NewName, NewContent)->AsShared()
		];
		MessageList.Push(InsertItem);
	}
	//滑动到最后
	ScrollBox->ScrollToEnd();
}

void SlAiChatRoomWidget::ScrollToEnd()
{
	ScrollBox->ScrollToEnd();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
