// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/SlAiPickupObject.h"

#include "SlAiDataHandle.h"
#include "SlAiTypes.h"


// Sets default values
ASlAiPickupObject::ASlAiPickupObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//实例化根组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//静态模型
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("PickupProfile"));

	//开启交互检测
	BaseMesh->SetGenerateOverlapEvents(true);

	IsDestroyNextTick = false;
}

// Called when the game starts or when spawned
void ASlAiPickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASlAiPickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDestroyNextTick)
	{
		GetWorld()->DestroyActor(this);
		IsDestroyNextTick = false;
	}
}

FText ASlAiPickupObject::GetInfoText() const
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	switch (SlAiDataHandle::Get()->CurCulture)
	{
	case ECultureTeam::EN:
		return ObjectAttr->EN;
		break;
	case ECultureTeam::ZH:
		return ObjectAttr->ZH;
		break;
	default: ;
	}
	return ObjectAttr->ZH;
}

int ASlAiPickupObject::PickUp()
{
	BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	if (GetWorld())
	{
		GetWorld()->DestroyActor(this);
	}
	return ObjectIndex;
}

