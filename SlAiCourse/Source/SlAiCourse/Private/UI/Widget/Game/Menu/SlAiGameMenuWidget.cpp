// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\..\..\..\Public\UI\Widget\Game\Menu\SlAiGameMenuWidget.h"

#include "SlAiDataHandle.h"
#include "SlAiGameOptionWidget.h"
#include "SlAiGameWidgetStyle.h"
#include "SlAiPlayerController.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Kismet/GameplayStatics.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameMenuWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[
		SAssignNew(RootBox, SBox)
		.WidthOverride(600.f)
		.HeightOverride(400.f)
		.Padding(FMargin(50.f))
		[
			SAssignNew(VertBox, SVerticalBox)
		]
		
	];

	InitWiget();
}

void SlAiGameMenuWidget::GameLose()
{
	VertBox->ClearChildren();

	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			QuitGameButton->AsShared()
		];
	
	RootBox->SetHeightOverride(200.f);
}

void SlAiGameMenuWidget::ResetMenu()
{
	VertBox->ClearChildren();

	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	
	RootBox->SetHeightOverride(400.f);
	
	//重置保存按钮
	SaveGameButton->SetVisibility(EVisibility::Visible);
	SaveGameText->SetText(NSLOCTEXT("SlAiGame", "SaveGame", "SaveGame"));
}

void SlAiGameMenuWidget::InitWiget()
{
	//设置
	MenuItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SlAiGameMenuWidget::OptionEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "GameOption", "GameOption"))
			.Font(GameStyle->Font_30)
		]
	);

	//保存
	MenuItemList.Add(
		SAssignNew(SaveGameButton, SButton)
		.OnClicked(this, &SlAiGameMenuWidget::SaveGameEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SAssignNew(SaveGameText, STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "SaveGame", "SaveGame"))
			.Font(GameStyle->Font_30)
		]
	);

	//离开游戏
	MenuItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SlAiGameMenuWidget::QuitGameEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "QuitGame", "QuitGame"))
			.Font(GameStyle->Font_30)
		]
	);

	//设置
	OptionItemList.Add(
		SNew(SlAiGameOptionWidget)
		.OnChangeCulture(this, &SlAiGameMenuWidget::ChangeCulture)
		.OnChangeVolume(this, &SlAiGameMenuWidget::ChangeVolume)
	);

	//返回
	OptionItemList.Add(
		SNew(SButton)
		.OnClicked(this, &SlAiGameMenuWidget::GoBackEvent)
		.HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("SlAiGame", "GoBack", "GoBack"))
			.Font(GameStyle->Font_30)
		]
	);

	//离开游戏
	SAssignNew(QuitGameButton, SButton)
	.OnClicked(this, &SlAiGameMenuWidget::QuitGameEvent)
	.HAlign(HAlign_Center).VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Text(NSLOCTEXT("SlAiGame", "QuitGame", "QuitGame"))
		.Font(GameStyle->Font_30)
	];

	//渲染按钮
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			(*It)->AsShared()
		];
	}
}

FReply SlAiGameMenuWidget::OptionEvent()
{
	//先清理
	VertBox->ClearChildren();
	
	//设置控件填充
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(3.2f)
		[
			OptionItemList[0]->AsShared()
		];
	
	VertBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(10.f)
		.FillHeight(1.f)
		[
			OptionItemList[1]->AsShared()
		];
	
	//设置高度
	RootBox->SetHeightOverride(520.f);
	return FReply::Handled();
}

FReply SlAiGameMenuWidget::SaveGameEvent()
{
	SaveGameDele.ExecuteIfBound();
	
	//设置保存不可用
	SaveGameButton->SetVisibility(EVisibility::HitTestInvisible);
	
	//保存完成
	SaveGameText->SetText(NSLOCTEXT("SlAiGame", "SaveCompleted", "SaveCompleted"));
	
	return FReply::Handled();
}

FReply SlAiGameMenuWidget::QuitGameEvent()
{
	Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GWorld, 0))->ConsoleCommand("quit");
	return FReply::Handled();
}

FReply SlAiGameMenuWidget::GoBackEvent()
{
	//先清理
	VertBox->ClearChildren();
	//填充
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(MenuItemList); It; ++It)
	{
		VertBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(10.f)
			.FillHeight(1.f)
			[
				(*It)->AsShared()
			];
	}
	//设置高度
	RootBox->SetHeightOverride(400.f);
	return FReply::Handled();
}

void SlAiGameMenuWidget::ChangeCulture(ECultureTeam Cultrue)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(Cultrue);
}

void SlAiGameMenuWidget::ChangeVolume(const float MusicVolume, const float SoundVolume)
{
	SlAiDataHandle::Get()->ResetMenuVolume(MusicVolume, SoundVolume);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
