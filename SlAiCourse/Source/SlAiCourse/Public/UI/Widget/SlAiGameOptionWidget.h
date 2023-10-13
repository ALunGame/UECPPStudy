// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"
#include "SSlider.h"

DECLARE_DELEGATE_OneParam(FChangeCulture, const ECultureTeam)
DECLARE_DELEGATE_TwoParams(FChangeVolume, const float, const float)

/**
 * 
 */
class SLAICOURSE_API SlAiGameOptionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiGameOptionWidget)
		{
		}

	//属性
	SLATE_EVENT(FChangeCulture,OnChangeCulture)
	SLATE_EVENT(FChangeVolume,OnChangeVolume)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	void InternationInit();
	
	void ZHCheckBoxStateChanged(ECheckBoxState NewState);
	
	void ENCheckBoxStateChanged(ECheckBoxState NewState);

	void MusicSoundInit();
	void MusicSliderValueChange(float Value);
	void SoundSliderValueChange(float Value);
private:
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	
	TSharedPtr<SCheckBox> ENCheckBox;
	TSharedPtr<SCheckBox> ZHCheckBox;
	FChangeCulture OnChangeCulture;

	TSharedPtr<SSlider> MusicSlider;
	TSharedPtr<SSlider> SoundSlider;
	TSharedPtr<STextBlock> MusicSliderValue;
	TSharedPtr<STextBlock> SoundSliderValue;
	FChangeVolume OnChangeVolume;
};
