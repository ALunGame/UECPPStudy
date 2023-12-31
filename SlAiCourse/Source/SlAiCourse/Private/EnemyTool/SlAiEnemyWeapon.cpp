﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTool/SlAiEnemyWeapon.h"

#include "ConstructorHelpers.h"
#include "SlAiPlayerCharacter.h"
#include "Components/BoxComponent.h"

ASlAiEnemyWeapon::ASlAiEnemyWeapon()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Wep_GreatAxe_01.SM_Wep_GreatAxe_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	BaseMesh->SetRelativeLocation(FVector(-38.f, 9.6f, 9.8f));
	BaseMesh->SetRelativeRotation(FRotator(-10.f, 76.5f, -99.f));
	BaseMesh->SetRelativeScale3D(FVector(0.75f));

	AffectCollision->SetRelativeLocation(FVector(0.f, 0.f, 158.f));
	AffectCollision->SetRelativeScale3D(FVector(1.125f, 0.22f, 1.f));
}

void ASlAiEnemyWeapon::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASlAiPlayerCharacter>(OtherActor))
	{
		Cast<ASlAiPlayerCharacter>(OtherActor)->AcceptDamage(20);
	}
}

