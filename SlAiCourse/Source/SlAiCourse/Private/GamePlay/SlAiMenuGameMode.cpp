// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GamePlay/SlAiMenuGameMode.h"

#include "SlAiMenuController.h"
#include "SlAiMenuHUD.h"

ASlAiMenuGameMode::ASlAiMenuGameMode()
{
	PlayerControllerClass = ASlAiMenuController::StaticClass();
	HUDClass = ASlAiMenuHUD::StaticClass();
}
