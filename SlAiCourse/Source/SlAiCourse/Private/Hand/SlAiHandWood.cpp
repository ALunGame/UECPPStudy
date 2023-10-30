// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandWood.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"


// Sets default values
ASlAiHandWood::ASlAiHandWood()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeLog_01.SM_Env_TreeLog_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));

	//碰撞
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
}

// Called when the game starts or when spawned
void ASlAiHandWood::BeginPlay()
{
	Super::BeginPlay();

	ObjectIndex = 1;
}


