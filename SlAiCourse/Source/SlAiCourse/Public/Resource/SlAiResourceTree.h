// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiResourceObject.h"
#include "SlAiResourceTree.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiResourceTree : public ASlAiResourceObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiResourceTree();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
