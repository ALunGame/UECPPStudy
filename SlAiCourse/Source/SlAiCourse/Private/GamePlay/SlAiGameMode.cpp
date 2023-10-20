﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"

#include "SlAiDataHandle.h"
#include "SlAiGameHUD.h"
#include "SlAiGameInstance.h"
#include "SlAiHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SlAiPlayerCharacter.h"
#include "..\..\Public\Player\SlAiPlayerController.h"
#include "Player/SlAiPlayerState.h"

ASlAiGameMode::ASlAiGameMode()
{
	//允许开始Tick函数
	PrimaryActorTick.bCanEverTick = true;
	
	//绑定自定义类
	HUDClass = ASlAiGameHUD::StaticClass();
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	
}

void ASlAiGameMode::BeginPlay()
{
	SlAiHelper::DebugWarning(FString("DataHandle:" + SlAiDataHandle::Get()->RecordName),30.f);
	SlAiHelper::DebugWarning(FString("GameInstance:" + Cast<USlAiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName),30.f);
}