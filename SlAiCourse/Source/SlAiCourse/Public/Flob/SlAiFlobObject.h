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

	void ThrowFlobObject(int ObjectId,float DirYaw);

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

	//玩家指针
	class ASlAiPlayerCharacter* SPCharacter;

	//定时检测
	FTimerHandle DetectTime;

	//销毁检测
	FTimerHandle DestroyTime;
	
private:
	
	void RenderTexture();

	//检测玩家
	void DetectPlayer();

	//销毁
	void DestroyEvent();
};
