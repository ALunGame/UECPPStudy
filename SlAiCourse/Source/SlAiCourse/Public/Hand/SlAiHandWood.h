// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiHandObject.h"
#include "SlAiHandWood.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiHandWood : public ASlAiHandObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiHandWood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
