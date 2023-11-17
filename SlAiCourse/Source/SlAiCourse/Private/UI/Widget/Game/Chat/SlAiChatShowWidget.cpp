// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiChatShowWidget.h"

#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "SMultiLineEditableText.h"

struct ChatShowItem
{
	float Alpha;
	//水平组件
	TSharedPtr<SHorizontalBox> CSBox;
	//名字
	TSharedPtr<STextBlock> CSName;
	//内容框
	TSharedPtr<SBorder> CSBorder;
	//内容
	TSharedPtr<SMultiLineEditableText> CSContent;

	ChatShowItem(const FSlateBrush* EmptyBrush, const FSlateFontInfo FontInfo)
	{
		Alpha = 0.f;

		SAssignNew(CSBox, SHorizontalBox)
		
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		[
			SAssignNew(CSName,STextBlock)
			.Font(FontInfo)
			.ColorAndOpacity(FLinearColor(0.f, 1.f, 0.f, 1.f))
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

	//激活组件
	TSharedPtr<SHorizontalBox> ActiveItem(FText NewName, FText NewContent)
	{
		CSName->SetText(NewName);
		CSContent->SetText(NewContent);
		Alpha = 1.f;
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Alpha));
		return CSBox;
	}

	//渐渐消失
	bool DeltaDisappear(float DeltaTime)
	{
		Alpha = FMath::Clamp<float>(Alpha - DeltaTime * 0.05f, 0.f, 1.f);
		CSName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 1.f, 0.f, Alpha)));
		CSBorder->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Alpha));
		if (Alpha == 0.f)
		{
			return true;
		}
		return false;
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SlAiChatShowWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.f)
		.HeightOverride(600.f)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			SAssignNew(ChatBox,SVerticalBox)
		]
	];

	Init();
}

void SlAiChatShowWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	TArray<TSharedPtr<ChatShowItem>> TempList;

	for (TArray<TSharedPtr<ChatShowItem>>::TIterator It(ActiveList); It; ++It)
	{
		if ((*It)->DeltaDisappear(InDeltaTime))
		{
			ChatBox->RemoveSlot((*It)->CSBox->AsShared());
			TempList.Push(*It);
		}
	}
	
	for (int i = 0; i < TempList.Num(); ++i) {
		ActiveList.Remove(TempList[i]);
		UnActiveList.Push(TempList[i]);
	}
}

void SlAiChatShowWidget::AddMessage(FText NewName, FText NewContent)
{
	TSharedPtr<ChatShowItem> InsertItem;

	if (UnActiveList.Num() > 0)
	{
		InsertItem = UnActiveList[0];
		UnActiveList.RemoveAt(0);
	}
	else
	{
		InsertItem = ActiveList[0];
		ActiveList.RemoveAt(0);
		//移除UI
		ChatBox->RemoveSlot(InsertItem->CSBox->AsShared());
	}

	//添加
	ChatBox->AddSlot()
	.HAlign(HAlign_Fill)
	.VAlign(VAlign_Fill)
	.FillHeight(1.f)
	[
		InsertItem->ActiveItem(NewName,NewContent)->AsShared()
	];
	
	ActiveList.Push(InsertItem);
}

void SlAiChatShowWidget::Init()
{
	for (int i = 0; i < 10; ++i)
	{
		UnActiveList.Add(MakeShareable(new ChatShowItem(&GameStyle->EmptyBrush,GameStyle->Font_16)));
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
