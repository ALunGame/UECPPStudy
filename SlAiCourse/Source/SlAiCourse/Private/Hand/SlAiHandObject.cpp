// Fill out your copyright notice in the Description page of Project Settings.


#include "Hand/SlAiHandObject.h"

#include "SlAiDataHandle.h"
#include "SlAiEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Hand/SlAiHandApple.h"
#include "Hand/SlAiHandAxe.h"
#include "Hand/SlAiHandHammer.h"
#include "Hand/SlAiHandMeat.h"
#include "Hand/SlAiHandNone.h"
#include "Hand/SlAiHandStone.h"
#include "Hand/SlAiHandSword.h"
#include "Hand/SlAiHandWood.h"


// Sets default values
ASlAiHandObject::ASlAiHandObject()
{
	//实例化根组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//静态模型
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("NoCollision"));

	//碰撞组件
	AffectCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AffectCollision"));
	AffectCollision->SetupAttachment(RootComponent);
	AffectCollision->SetCollisionProfileName(FName("ToolProfile"));

	//初始时关闭Overlay检测
	AffectCollision->SetGenerateOverlapEvents(false);

	//绑定检测方法
	FScriptDelegate OverlayBegin;
	OverlayBegin.BindUFunction(this,"OnOverlayBegin");
	AffectCollision->OnComponentBeginOverlap.Add(OverlayBegin);

	FScriptDelegate OverlayEnd;
	OverlayEnd.BindUFunction(this,"OnOverlayEnd");
	AffectCollision->OnComponentEndOverlap.Add(OverlayEnd);
}

void ASlAiHandObject::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASlAiEnemyCharacter>(OtherActor))
	{
		TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
		Cast<ASlAiEnemyCharacter>(OtherActor)->AcceptDamage(ObjectAttr->AnimalAttack);
	}
}

void ASlAiHandObject::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ASlAiHandObject::ChangeOverlayDetect(bool IsOpen)
{
	AffectCollision->SetGenerateOverlapEvents(IsOpen);
}

TSubclassOf<AActor> ASlAiHandObject::SpawnHandObject(int ObjectIndex)
{
	switch (ObjectIndex)
	{
	case 0:
		return ASlAiHandNone::StaticClass();
	case 1:
		return ASlAiHandWood::StaticClass();
	case 2:
		return ASlAiHandStone::StaticClass();
	case 3:
		return ASlAiHandApple::StaticClass();
	case 4:
		return ASlAiHandMeat::StaticClass();
	case 5:
		return ASlAiHandAxe::StaticClass();
	case 6:
		return ASlAiHandHammer::StaticClass();
	case 7:
		return ASlAiHandSword::StaticClass();
	}

	return ASlAiHandNone::StaticClass();
}

