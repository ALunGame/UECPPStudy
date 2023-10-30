// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiPickupObject.h"
#include "SlAiPickupStone.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiPickupStone : public ASlAiPickupObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiPickupStone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
