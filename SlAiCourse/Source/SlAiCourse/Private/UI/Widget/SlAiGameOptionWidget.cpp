// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameOptionWidget.h"

#include "SlAiDataHandle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiGameOptionWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//获得委托
	OnChangeCulture = InArgs._OnChangeCulture;
	OnChangeVolume = InArgs._OnChangeVolume;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.F)
		.HeightOverride(300.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->GameOptionBGBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(50.f))
			[
				SNew(SVerticalBox)

				//第一行中英文
				+SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.FillWidth(1.F)
					[
						SAssignNew(ZHCheckBox,SCheckBox)
						.OnCheckStateChanged(this,&SlAiGameOptionWidget::ZHCheckBoxStateChanged)
						[
							SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.ColorAndOpacity(MenuStyle->FontColor_Black)
							.Text(NSLOCTEXT("SlAiMenu","Chinese","Chinese"))
						]
					]

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.FillWidth(1.F)
					[
						SAssignNew(ENCheckBox,SCheckBox)
						.OnCheckStateChanged(this,&SlAiGameOptionWidget::ENCheckBoxStateChanged)
						[
							SNew(STextBlock)
							.Font(MenuStyle->Font_40)
							.ColorAndOpacity(MenuStyle->FontColor_Black)
							.Text(NSLOCTEXT("SlAiMenu","English","English"))
						]
					]
				]

				//第二行声音
				+SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SOverlay)

					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Font(MenuStyle->Font_40)
						.ColorAndOpacity(MenuStyle->FontColor_Black)
						.Text(NSLOCTEXT("SlAiMenu","Music","Music"))
					]

					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.WidthOverride(240.f)
						[
							SNew(SOverlay)
							
							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Padding(FMargin(30.f,0.f))
							[
								SNew(SImage)
								.Image(&MenuStyle->SliderBarBrush)
							]

							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							[
								SAssignNew(MusicSlider,SSlider)
								.Style(&MenuStyle->SliderStyle)
								.OnValueChanged(this,&SlAiGameOptionWidget::MusicSliderValueChange)
							]
						]
					]

					//滑动值
					+SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					[
						SAssignNew(MusicSliderValue,STextBlock)
						.Font(MenuStyle->Font_40)
						.ColorAndOpacity(MenuStyle->FontColor_Black)
					]
				]

				//第三行音效
				+SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.FillHeight(1.f)
				[
					SNew(SOverlay)

					+SOverlay::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Center)
					[
						SNew(STextBlock)
						.Font(MenuStyle->Font_40)
						.ColorAndOpacity(MenuStyle->FontColor_Black)
						.Text(NSLOCTEXT("SlAiMenu","Sound","Sound"))
					]

					+SOverlay::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SBox)
						.WidthOverride(240.f)
						[
							SNew(SOverlay)
							
							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Padding(FMargin(30.f,0.f))
							[
								SNew(SImage)
								.Image(&MenuStyle->SliderBarBrush)
							]

							+SOverlay::Slot()
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							[
								SAssignNew(SoundSlider,SSlider)
								.Style(&MenuStyle->SoundSliderStyle)
								.OnValueChanged(this,&SlAiGameOptionWidget::SoundSliderValueChange)
							]
						]
					]

					//滑动值
					+SOverlay::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					[
						SAssignNew(SoundSliderValue,STextBlock)
						.Font(MenuStyle->Font_40)
						.ColorAndOpacity(MenuStyle->FontColor_Black)
					]
				]

				// +SVerticalBox::Slot()
				// .HAlign(HAlign_Fill)
				// .VAlign(VAlign_Fill)
				// .FillHeight(1.f)
				// [
				//
				// ]
			]
		]
	];

	InternationInit();
	MusicSoundInit();
}

void SlAiGameOptionWidget::InternationInit()
{
	ZHCheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBrush);
	ZHCheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBrush);
	ZHCheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBrush);
	ZHCheckBox->SetCheckedImage(&MenuStyle->CheckedBrush);
	ZHCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBrush);
	ZHCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBrush);

	ENCheckBox->SetUncheckedImage(&MenuStyle->UnCheckedBrush);
	ENCheckBox->SetUncheckedHoveredImage(&MenuStyle->UnCheckedBrush);
	ENCheckBox->SetUncheckedPressedImage(&MenuStyle->UnCheckedBrush);
	ENCheckBox->SetCheckedImage(&MenuStyle->CheckedBrush);
	ENCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBrush);
	ENCheckBox->SetCheckedHoveredImage(&MenuStyle->CheckedBrush);

	switch (SlAiDataHandle::Get()->CurCulture)
	{
	case ECultureTeam::EN:
		ZHCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
		ENCheckBox->SetIsChecked(ECheckBoxState::Checked);
		break;
	case ECultureTeam::ZH:
		ZHCheckBox->SetIsChecked(ECheckBoxState::Checked);
		ENCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
		break;
	}
}

void SlAiGameOptionWidget::ZHCheckBoxStateChanged(ECheckBoxState NewState)
{
	OnChangeCulture.ExecuteIfBound(ECultureTeam::ZH);
	ZHCheckBox->SetIsChecked(ECheckBoxState::Checked);
	ENCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
}

void SlAiGameOptionWidget::ENCheckBoxStateChanged(ECheckBoxState NewState)
{
	OnChangeCulture.ExecuteIfBound(ECultureTeam::EN);
	ZHCheckBox->SetIsChecked(ECheckBoxState::Unchecked);
	ENCheckBox->SetIsChecked(ECheckBoxState::Checked);
}

void SlAiGameOptionWidget::MusicSoundInit()
{
	MusicSlider->SetValue(SlAiDataHandle::Get()->MusicVolume);
	MusicSliderValue->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SlAiDataHandle::Get()->MusicVolume*100))+FString("%")));

	SoundSlider->SetValue(SlAiDataHandle::Get()->SoundVolume);
	SoundSliderValue->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(SlAiDataHandle::Get()->SoundVolume*100))+FString("%")));
}

void SlAiGameOptionWidget::MusicSliderValueChange(float Value)
{
	OnChangeVolume.ExecuteIfBound(Value,-1);
	MusicSliderValue->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(Value*100))+FString("%")));
}

void SlAiGameOptionWidget::SoundSliderValueChange(float Value)
{
	OnChangeVolume.ExecuteIfBound(-1,Value);
	SoundSliderValue->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(Value*100))+FString("%")));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
