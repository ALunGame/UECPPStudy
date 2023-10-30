// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandSword.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"


// Sets default values
ASlAiHandSword::ASlAiHandSword()
{
	//模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Wep_Sword_01.SM_Wep_Sword_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(-15.f, 1.f, 2.f));
	BaseMesh->SetRelativeRotation(FRotator(-20.f, 90.f, -90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.f));

	//碰撞
	AffectCollision->SetRelativeLocation(FVector(62.f, 1.f, 2.f));
	AffectCollision->SetRelativeRotation(FRotator(0.f, 0.f, 20.f));
	AffectCollision->SetRelativeScale3D(FVector(1.5f, 0.19f, 0.1f));
}

// Called when the game starts or when spawned
void ASlAiHandSword::BeginPlay()
{
	Super::BeginPlay();

	ObjectIndex = 7;
	
}
