// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMenuWidget.h"

#include "SlAiChooseRecordWidget.h"
#include "SlAiDataHandle.h"
#include "SlAiGameOptionWidget.h"
#include "SlAiMenuItemWidget.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiNewGameWidget.h"
#include "SlAiStyle.h"
#include "SlAiTypes.h"
#include "SlateOptMacros.h"
#include "Common/SlAiHelper.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSlAiMenuWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	ChildSlot
	[
		SAssignNew(RootSizeBox,SBox)
		.WidthOverride(600.f).HeightOverride(510.f)
		[
			SNew(SOverlay)
			
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(0,50.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuBackgroundBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0,25.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->LeftIconBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0,25.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->RightIconBrush)
			]
			
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SBox)
				.WidthOverride(400.f).HeightOverride(100.f)
				[
					SNew(SBorder)
					.BorderImage(&MenuStyle->TitleBorderBrush)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(TitleText,STextBlock)
						.Font(MenuStyle->FontInfo)
						.Text(NSLOCTEXT("SlAiMenu","Menu","Menu"))
					]
				]
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(FMargin(0.f,130.f,0.f,0.f))
			[
				SAssignNew(ContentBox,SVerticalBox)
			]
		]
		
	];

	//添加按钮
	ContentBox->AddSlot()
	[

		SNew(SlAiChooseRecordWidget)
		//SNew(SlAiNewGameWidget)
	
		// SNew(SlAiGameOptionWidget)
		// .OnChangeCulture(this,&SSlAiMenuWidget::OnChangeCulture)
		// .OnChangeVolume(this,&SSlAiMenuWidget::OnChangeVolume)
		// SNew(SlAiMenuItemWidget)
		// .ItemText(NSLOCTEXT("SlAiMenu","StartGame","StartGame"))
		// .ItemType(EMenuItem::StartGame)
		// .OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked)
	];
}

void SSlAiMenuWidget::MenuItemOnClicked(EMenuItem::Type ItemType)
{
	SlAiHelper::Debug("StartGame");
}

void SSlAiMenuWidget::OnChangeCulture(ECultureTeam NewCulture)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(NewCulture);
}

void SSlAiMenuWidget::OnChangeVolume(float MusicVolume, float SoundVolume)
{
	if (MusicVolume > 0)
	{
		SlAiDataHandle::Get()->SetMusicVolume(MusicVolume);
	}

	if (SoundVolume > 0)
	{
		SlAiDataHandle::Get()->SetSoundVolume(SoundVolume);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
