// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUD.h"

#include "SlAiGameHUDWidget.h"
#include "SWeakWidget.h"

ASlAiGameHUD::ASlAiGameHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		SAssignNew(GameHUDWidget,SlAiGameHUDWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}
}
