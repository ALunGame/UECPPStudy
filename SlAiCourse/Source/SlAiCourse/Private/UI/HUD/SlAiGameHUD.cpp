﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameHUD.h"

#include "SlAiGameHUDWidget.h"
#include "SlAiPlayerState.h"
#include "SlAiShortcutWidget.h"
#include "SWeakWidget.h"
#include "Interactive/SlAiRayInfoWidget.h"
#include "Kismet/GameplayStatics.h"

ASlAiGameHUD::ASlAiGameHUD()
{
	if (GEngine && GEngine->GameViewport)
	{
		SAssignNew(GameHUDWidget,SlAiGameHUDWidget);
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(GameHUDWidget.ToSharedRef()));
	}
}

void ASlAiGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ASlAiGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	SlAiHelper::Debug("ASlAiGameHUD::BeginPlay()",60.f);
	if (!GameMode)
	{
		return;
	}
	
	//确保组件都已经实现
	GameMode->InitGameplayMode();
	//注册快捷栏委托
	GameHUDWidget->ShortcutWidget->RegisterShortcutContainer.BindUObject(GameMode->SPState,&ASlAiPlayerState::OnRegisterShortcutContainer);
	//注册涉嫌信息委托
	GameHUDWidget->RayInfoWidget->RegisterRayInfoEvent.BindUObject(GameMode->SPState,&ASlAiPlayerState::RegisterRayInfoEvent);
}
