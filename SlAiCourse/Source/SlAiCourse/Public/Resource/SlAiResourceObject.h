// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiResourceObject.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiResourceObject : public AActor
{
	GENERATED_BODY()

public:

	//Id
	int ResourceIndex;
	
public:
	// Sets default values for this actor's properties
	ASlAiResourceObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	
	//根组件
	USceneComponent* RootScene;

	//模型
	UStaticMeshComponent* BaseMesh;

	//保存资源地址，用于刷新资源
	TArray<FString> ResourcePath;
};
