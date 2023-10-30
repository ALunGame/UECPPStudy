// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/SlAiResourceTree.h"

#include "ConstructorHelpers.h"


// Sets default values
ASlAiResourceTree::ASlAiResourceTree()
{
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_07.SM_Env_Tree_07'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_016.SM_Env_Tree_016'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_014_Snow.SM_Env_Tree_014_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_012.SM_Env_Tree_012'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreePine_01_Snow.SM_Env_TreePine_01_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_01_Snow.SM_Env_Tree_01_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_06_Snow.SM_Env_Tree_06_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeDead_02_Snow.SM_Env_TreeDead_02_Snow'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_TreeDead_01.SM_Env_TreeDead_01'"));
	ResourcePath.Add(FString("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Env_Tree_012_Snow.SM_Env_Tree_012_Snow'"));

	FRandomStream Stream;

	//新的随机种子
	Stream.GenerateNewSeed();

	int RangeIndex = Stream.RandRange(0,ResourcePath.Num()-1);

	//模型
	ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(*ResourcePath[RangeIndex]);
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	ResourceIndex = 1;
}

// Called when the game starts or when spawned
void ASlAiResourceTree::BeginPlay()
{
	Super::BeginPlay();
}

