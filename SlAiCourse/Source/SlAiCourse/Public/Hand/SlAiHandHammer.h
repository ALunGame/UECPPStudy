// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiHandObject.h"
#include "SlAiHandHammer.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiHandHammer : public ASlAiHandObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiHandHammer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Mesh)
		UStaticMeshComponent* ExtendMesh;
};
