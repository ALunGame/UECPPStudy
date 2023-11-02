// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/SlAiPickupWood.h"

#include "ConstructorHelpers.h"


// Sets default values
ASlAiPickupWood::ASlAiPickupWood()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_Loghalf_01.SM_Prop_Loghalf_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeScale3D(FVector(0.4f));

	//随机种子
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int RangeRoate = Stream.RandRange(-180,180);
	
	BaseMesh->SetRelativeRotation(FRotator(0,RangeRoate,0));

	ObjectIndex = 1;
}

// Called when the game starts or when spawned
void ASlAiPickupWood::BeginPlay()
{
	Super::BeginPlay();
	
}


