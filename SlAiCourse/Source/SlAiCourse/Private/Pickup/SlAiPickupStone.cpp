// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/SlAiPickupStone.h"

#include "ConstructorHelpers.h"


// Sets default values
ASlAiPickupStone::ASlAiPickupStone()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.5f));

	ObjectIndex = 2;
}

// Called when the game starts or when spawned
void ASlAiPickupStone::BeginPlay()
{
	Super::BeginPlay();
}

