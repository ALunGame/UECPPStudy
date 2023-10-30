// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandNone.h"

#include "Components/BoxComponent.h"


// Sets default values
ASlAiHandNone::ASlAiHandNone()
{
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
}

// Called when the game starts or when spawned
void ASlAiHandNone::BeginPlay()
{
	Super::BeginPlay();
	
}


