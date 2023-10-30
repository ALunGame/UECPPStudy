// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandMeat.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"


// Sets default values
ASlAiHandMeat::ASlAiHandMeat()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_Meat_02.SM_Prop_Meat_02'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(6.f, -7.044f, 2.62f));
	BaseMesh->SetRelativeRotation(FRotator(-50.f, 90.f, 0.f));
	BaseMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	//碰撞
	AffectCollision->SetBoxExtent(FVector(10.f, 10.f, 10.f));
	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
}

// Called when the game starts or when spawned
void ASlAiHandMeat::BeginPlay()
{
	Super::BeginPlay();

	ObjectIndex = 4;
}

