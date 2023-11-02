// Fill out your copyright notice in the Description page of Project Settings.


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

	// PlayerControllerClass = ASlAiPlayerController::StaticClass();
	// PlayerStateClass = ASlAiPlayerState::StaticClass();
	
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	
}

void ASlAiGameMode::InitGameplayMode()
{
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	SPState = Cast<ASlAiPlayerState>(SPController->PlayerState);
	//SPState = Cast<ASlAiPlayerState>(SPController.state);
}

void ASlAiGameMode::BeginPlay()
{
	//SlAiHelper::DebugWarning(FString("DataHandle:" + SlAiDataHandle::Get()->RecordName),30.f);
	//SlAiHelper::DebugWarning(FString("GameInstance:" + Cast<USlAiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName),30.f);

	SlAiDataHandle::Get()->InitGameData();

	SlAiHelper::DebugWarning(FString("InitGameData:" + SlAiDataHandle::Get()->RecordName),30.f);
	
	if (!SPController)
	{
		InitGameplayMode();
	}
}
