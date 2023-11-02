// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiFlobObject.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiFlobObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiFlobObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//初始化
	void CreateFlobObject(int ObjectId);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class UBoxComponent* BoxCollision;

	//模型
	class UStaticMeshComponent* BaseMesh;

	//物品Id
	int ObjectIndex;

	class UTexture* ObjectIconTex;

	class UMaterialInstanceDynamic* ObjectIconMatDynamic;
	
private:
	
	void RenderTexture();
};
