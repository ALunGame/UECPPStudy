// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiPickupObject.h"
#include "SlAiPickupWood.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiPickupWood : public ASlAiPickupObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiPickupWood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
