// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiGameMode.h"
#include "GameFramework/HUD.h"
#include "SlAiGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	ASlAiGameMode* GameMode;
	
public:
	ASlAiGameHUD();

	TSharedPtr<class SlAiGameHUDWidget> GameHUDWidget;

protected:

	virtual void BeginPlay() override;
};
