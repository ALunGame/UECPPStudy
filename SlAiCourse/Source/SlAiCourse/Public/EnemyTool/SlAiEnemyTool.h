// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlAiEnemyTool.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiEnemyTool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlAiEnemyTool();

	//开启碰撞检测
	void ChangeOverlayDetect(bool IsOpen);

	//创建武器
	static TSubclassOf<AActor> SpawnEnemyWeapon();

	//创建武器
	static TSubclassOf<AActor> SpawnEnemySheild();
	
protected:
	
	//碰撞开始
	UFUNCTION()
	virtual void OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//碰撞结束
	UFUNCTION()
	virtual void OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex); 
	
protected:

	//根组件
	class USceneComponent* RootScene;

	//静态模型
	UPROPERTY(EditAnywhere,Category="SlAi")
	class UStaticMeshComponent* BaseMesh;

	//盒子碰撞盒
	UPROPERTY(EditAnywhere,Category="SlAi")
	class UBoxComponent* AffectCollision;
};
