// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUDWidget.h"

#include "SDPIScaler.h"
#include "SlAiChatShowWidget.h"
#include "SlAiMinMapWidget.h"
#include "SlAiPointerWidget.h"
#include "SlAiShortcutWidget.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Bag/SlAiBagWidget.h"
#include "Chat/SlAiChatRoomWidget.h"
#include "Interactive/SlAiRayInfoWidget.h"
#include "Menu/SlAiGameMenuWidget.h"
#include "Player/SlAiPlayerStateWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameHUDWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	//绑定UI缩放规则
	UIScaler.Bind(this,&SlAiGameHUDWidget::GetUIScaler);
	
	ChildSlot
	[
		SNew(SDPIScaler).DPIScale(UIScaler)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ShortcutWidget,SlAiShortcutWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SAssignNew(RayInfoWidget,SlAiRayInfoWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(PointerWidget,SlAiPointerWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SAssignNew(PlayerStateWidget,SlAiPlayerStateWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SAssignNew(MinMapWidget,SlAiMinMapWidget)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			.Padding(FMargin(20.f,0.f,0.f,15.f))
			[
				SAssignNew(ChatShowWidget,SlAiChatShowWidget)
			]

			//暗黑色遮罩
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(BlackShade,SBorder)
				//黑色透明
				.ColorAndOpacity(TAttribute<FLinearColor>(FLinearColor(0.2f,0.2f,0.2f,0.5f)))
				//开始不显示
				.Visibility(EVisibility::Hidden)
				[
					SNew(SImage)
				]
			]

			//GameMenu
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(GameMenuWidget,SlAiGameMenuWidget)
				.Visibility(EVisibility::Hidden)
			]
			
			//ChatRoom
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			[
				SAssignNew(ChatRoomWidget,SlAiChatRoomWidget)
				.Visibility(EVisibility::Hidden)
			]

			//Bag
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(BagWidget,SlAiBagWidget).UIScaler(UIScaler)
				.Visibility(EVisibility::Hidden)
			]
		]
	];

	InitUIMap();
}

void SlAiGameHUDWidget::ShowGameUI(EGameUIType::Type PreUI, EGameUIType::Type NextUI)
{
	//如果前一个是Game
	if (PreUI == EGameUIType::Game)
	{
		BlackShade->SetVisibility(EVisibility::Visible);
	}
	else
	{
		//隐藏当前的
		UIMap.Find(PreUI)->Get()->SetVisibility(EVisibility::Hidden);
	}

	//如果前一个是Game
	if (NextUI == EGameUIType::Game)
	{
		BlackShade->SetVisibility(EVisibility::Hidden);
	}
	else
	{
		//隐藏当前的
		UIMap.Find(NextUI)->Get()->SetVisibility(EVisibility::Visible);
		//聊天室
		if (NextUI == EGameUIType::ChatRoom)
		{
			ChatRoomWidget->ScrollToEnd();
		}
		if (NextUI == EGameUIType::Lose)
		{
			GameMenuWidget->GameLose();
		}
		if (NextUI == EGameUIType::Pause)
		{
			GameMenuWidget->ResetMenu();
		}
	}
}

void SlAiGameHUDWidget::InitUIMap()
{
	UIMap.Add(EGameUIType::Pause,GameMenuWidget);
	UIMap.Add(EGameUIType::Bag,BagWidget);
	UIMap.Add(EGameUIType::ChatRoom,ChatRoomWidget);
	UIMap.Add(EGameUIType::Lose,GameMenuWidget);

	//委托
	ChatRoomWidget->PushMessage.BindRaw(ChatShowWidget.Get(),&SlAiChatShowWidget::AddMessage);
}

float SlAiGameHUDWidget::GetUIScaler() const
{
	return GetViewportSize().Y / 1080.f;
}

FVector2d SlAiGameHUDWidget::GetViewportSize() const
{
	FVector2d Result(1920,1080);
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(Result);
	}
	return Result;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
