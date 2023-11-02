// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource/SlAiResourceObject.h"

#include "SlAiDataHandle.h"
#include "SlAiTypes.h"
#include "Flob/SlAiFlobObject.h"


// Sets default values
ASlAiResourceObject::ASlAiResourceObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//实例化根组件
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	//静态模型
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionProfileName(FName("ResourceProfile"));

	//开启交互检测
	BaseMesh->SetGenerateOverlapEvents(true);

	IsInit = false;
}

// Called when the game starts or when spawned
void ASlAiResourceObject::BeginPlay()
{
	Super::BeginPlay();

	SlAiHelper::DebugWarning(FString("ASlAiResourceObject::BeginPlay():"),30.f);
	
	//TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	//HP = BaseHP = ResourceAttr->HP;
}

void ASlAiResourceObject::CreateFlobObject()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	//遍历生成掉落物
	for(TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It)
	{
		//随机生成数量
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int Num = Stream.RandRange((*It)[1],(*It)[2]);

		if (GetWorld())
		{
			for (int i = 0; i < Num; ++i)
			{
				ASlAiFlobObject* FlobObject = GetWorld()->SpawnActor<ASlAiFlobObject>(GetActorLocation(),FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

// Called every frame
void ASlAiResourceObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsInit)
	{
		TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
		HP = BaseHP = ResourceAttr->HP;
		IsInit = true;
	}
}

FText ASlAiResourceObject::GetInfoText() const
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	switch (SlAiDataHandle::Get()->CurCulture)
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
		break;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
		break;
	default: ;
	}
	return ResourceAttr->ZH;
}

EResourceType::Type ASlAiResourceObject::GetResourceType()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);
	return ResourceAttr->ResourceType;
}

ASlAiResourceObject* ASlAiResourceObject::TakeObjectDamage(int Damage)
{
	HP = FMath::Clamp<int>(HP -  Damage, 0, BaseHP);

	if (HP <= 0)
	{
		//检测失效
		BaseMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		//创建掉落物
		CreateFlobObject();
		//销毁物品
		GetWorld()->DestroyActor(this);
	}

	return this;
}

float ASlAiResourceObject::GetHPRange()
{
	return FMath::Clamp<float>((float)HP / (float)BaseHP, 0.f, 1.f);
}

