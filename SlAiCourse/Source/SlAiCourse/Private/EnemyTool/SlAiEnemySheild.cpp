﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTool/SlAiEnemySheild.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"

ASlAiEnemySheild::ASlAiEnemySheild()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Wep_Sheild_01.SM_Wep_Sheild_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(53.f, -3.f, -9.f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));

	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 43.f));
	AffectCollision->SetRelativeScale3D(FVector(0.8125f, 0.156f, 1.344f));
}

