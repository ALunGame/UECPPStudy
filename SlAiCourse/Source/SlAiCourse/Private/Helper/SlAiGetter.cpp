// Fill out your copyright notice in the Description page of Project Settings.


#include "Helper/SlAiGetter.h"

UGameInstance* SlAiGetter::GetGameInstance()
{
	UGameInstance* TGameInstance = nullptr;
#if WITH_EDITOR
	UUnrealEdEngine* engine = Cast<UUnrealEdEngine>(GEngine);
	if (engine && engine->PlayWorld)
		TGameInstance = engine->PlayWorld->GetGameInstance();
#else
	UGameEngine* engine = Cast<UGameEngine>(GEngine);
	if (engine)
		TGameInstance = engine->GameInstance;
#endif
	return TGameInstance;
}
