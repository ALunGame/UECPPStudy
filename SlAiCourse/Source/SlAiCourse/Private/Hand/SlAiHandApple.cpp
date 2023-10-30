// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandApple.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"


// Sets default values
ASlAiHandApple::ASlAiHandApple()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Item_Fruit_02.SM_Item_Fruit_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(-8.f, -3.f, 7.f));
	BaseMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));


	//碰撞
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
}

// Called when the game starts or when spawned
void ASlAiHandApple::BeginPlay()
{
	Super::BeginPlay();

	ObjectIndex = 3;
}

