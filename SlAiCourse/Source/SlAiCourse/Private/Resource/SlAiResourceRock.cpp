// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/SlAiResourceRock.h"

#include "ConstructorHelpers.h"


// Sets default values
ASlAiResourceRock::ASlAiResourceRock()
{
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_02.SM_Env_Rock_02'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_03.SM_Env_Rock_03'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_03_Snow.SM_Env_Rock_03_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_04.SM_Env_Rock_04'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Rock_04_Snow.SM_Env_Rock_04_Snow'"));

	//随机种子
	FRandomStream Stream;
	Stream.GenerateNewSeed();

	int RangeIndex = Stream.RandRange(0,ResourcePath.Num()-1);

	//模型
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(*ResourcePath[RangeIndex]);
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	ResourceIndex = 2;
}

// Called when the game starts or when spawned
void ASlAiResourceRock::BeginPlay()
{
	Super::BeginPlay();
	
}

