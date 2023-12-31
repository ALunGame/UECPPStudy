﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SlAiGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "SlAi")
		FString GameName;

	void LoadLevel(const FName LevelName);
};
