// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandHammer.h"

#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundWave.h"


// Sets default values
ASlAiHandHammer::ASlAiHandHammer()
{
	//基础模型
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Bld_FencePost_01.SM_Bld_FencePost_01'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);
	BaseMesh->SetRelativeLocation(FVector(35.f, 0.f, 3.f));
	BaseMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

	//拼接模型
	ExtendMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExtendMesh"));
	ExtendMesh->SetupAttachment(RootComponent);
	ExtendMesh->SetCollisionProfileName(FName("NoCollision"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticExtendMesh(TEXT("StaticMesh'/Game/Res/PolygonAdventure/Meshes/SM_Prop_StoneBlock_01.SM_Prop_StoneBlock_01'"));
	ExtendMesh->SetStaticMesh(StaticExtendMesh.Object);
	ExtendMesh->SetRelativeLocation(FVector(33.f, 1.f, 3.f));
	ExtendMesh->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));
	ExtendMesh->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	
	//碰撞
	AffectCollision->SetRelativeLocation(FVector(26.f, 1.f, 3.f));
	AffectCollision->SetRelativeScale3D(FVector(0.22f, 0.44f, 0.31f));
	
}

// Called when the game starts or when spawned
void ASlAiHandHammer::BeginPlay()
{
	Super::BeginPlay();

	ObjectIndex = 6;

	OverlaySound = LoadObject<USoundWave>(nullptr, TEXT("SoundWave'/Game/Res/Sound/GameSound/Hammer.Hammer'"));
}


