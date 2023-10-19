// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameInstance.h"

#include "Kismet/GameplayStatics.h"

void USlAiGameInstance::LoadLevel(const FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}
