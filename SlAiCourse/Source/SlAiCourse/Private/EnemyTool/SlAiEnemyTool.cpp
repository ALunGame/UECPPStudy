// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTool/SlAiEnemyTool.h"

#include "SlAiEnemySheild.h"
#include "SlAiEnemyWeapon.h"
#include "Components/BoxComponent.h"


// Sets default values
ASlAiEnemyTool::ASlAiEnemyTool()
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
	AffectCollision->SetCollisionProfileName(FName("EnemyToolProfile"));

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

void ASlAiEnemyTool::ChangeOverlayDetect(bool IsOpen)
{
	AffectCollision->SetGenerateOverlapEvents(IsOpen);
}

void ASlAiEnemyTool::OnOverlayBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ASlAiEnemyTool::OnOverlayEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

TSubclassOf<AActor> ASlAiEnemyTool::SpawnEnemyWeapon()
{
	return ASlAiEnemyWeapon::StaticClass();
}

TSubclassOf<AActor> ASlAiEnemyTool::SpawnEnemySheild()
{
	return ASlAiEnemySheild::StaticClass();
}

