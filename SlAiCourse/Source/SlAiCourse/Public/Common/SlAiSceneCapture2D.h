// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SlAiSceneCapture2D.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiSceneCapture2D : public ASceneCapture2D
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiSceneCapture2D();

	UTextureRenderTarget2D* GetMiniMapTex();

	void UpdateTransform(FVector NormalLocation, FRotator NormalRotator);

	void UpdateMiniMapWidth(int Delta);

	float GetMapSize();
	
private:

	UTextureRenderTarget2D* MiniMapTex;
};
