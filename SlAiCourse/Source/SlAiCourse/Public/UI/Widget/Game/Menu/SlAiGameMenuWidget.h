// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"


DECLARE_DELEGATE(FSaveGameDelegate)

/**
 * 
 */
class SLAICOURSE_API SlAiGameMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiGameMenuWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	
	//游戏失败
	void GameLose();

	//重置
	void ResetMenu();

public:
	
	FSaveGameDelegate SaveGameDele;

private:

	void InitWiget();
	
	FReply OptionEvent();
	FReply SaveGameEvent();
	FReply QuitGameEvent();
	FReply GoBackEvent();

	//语言
	void ChangeCulture(ECultureTeam Cultrue);
	//音量
	void ChangeVolume(const float MusicVolume, const float SoundVolume);

	
private:

	TSharedPtr<class SBox> RootBox;
	
	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	TSharedPtr<class SVerticalBox> VertBox;

	TSharedPtr<SButton> SaveGameButton;

	TSharedPtr<class STextBlock> SaveGameText;

	TSharedPtr<class SButton> QuitGameButton;
	
	TArray<TSharedPtr<SCompoundWidget>> MenuItemList;

	TArray<TSharedPtr<SCompoundWidget>> OptionItemList;
};
