// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "GameFramework/Actor.h"
#include "SlAiResourceObject.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiResourceObject : public AActor
{
	GENERATED_BODY()

public:

	//Id
	int ResourceIndex;

	bool IsDestroyNextTick;
	
public:
	// Sets default values for this actor's properties
	ASlAiResourceObject();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//获取物品信息
	FText GetInfoText() const;

	//获得资源类型
	EResourceType::Type GetResourceType();

	//受到伤害
	ASlAiResourceObject* TakeObjectDamage(int Damage);

	//获得血量范围
	float GetHPRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateFlobObject();

protected:
	
	//根组件
	USceneComponent* RootScene;

	//模型
	UStaticMeshComponent* BaseMesh;

	//保存资源地址，用于刷新资源
	TArray<FString> ResourcePath;

	bool IsInit;
	
	int HP;

	int BaseHP;
};
