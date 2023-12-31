﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiPickupObject.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiPickupObject : public AActor
{
	GENERATED_BODY()

public:

	int ObjectIndex;

	bool IsDestroyNextTick;

public:
	// Sets default values for this actor's properties
	ASlAiPickupObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//获取物品信息
	FText GetInfoText() const;

	//拾取
	int PickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	//根组件
	USceneComponent* RootScene;

	//模型
	UStaticMeshComponent* BaseMesh;

};
