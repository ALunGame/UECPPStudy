// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/SlAiStyle.h"

#include "SlateGameResources.h"
#include "SlateStyleRegistry.h"

TSharedPtr<FSlateStyleSet> SlAiStyle::SlAiStyleInstance = nullptr;

void SlAiStyle::Init()
{
	if (!SlAiStyleInstance.IsValid())
	{
		SlAiStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*SlAiStyleInstance);
	}
}

FName SlAiStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SlAiStyle"));
	return StyleSetName;
}

void SlAiStyle::ShutDown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*SlAiStyleInstance);
	ensure(SlAiStyleInstance.IsUnique());
	SlAiStyleInstance.Reset();
}

const ISlateStyle& SlAiStyle::Get()
{
	return *SlAiStyleInstance;
}

TSharedRef<FSlateStyleSet> SlAiStyle::Create()
{
	TSharedRef<FSlateStyleSet> styleRef = FSlateGameResources::New(SlAiStyle::GetStyleSetName(),"/Game/UI/Style","/Game/UI/Style");
	// styleRef->Set("MenuItemFont",FSlateFontInfo("Roboto-Regular.ttf",50));
	return styleRef;
}
