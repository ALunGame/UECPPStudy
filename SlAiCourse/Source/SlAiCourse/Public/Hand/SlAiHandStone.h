// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiHandObject.h"
#include "SlAiHandStone.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiHandStone : public ASlAiHandObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiHandStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
