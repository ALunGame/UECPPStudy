// Fill out your copyright notice in the Description page of Project Settings.


#include "Flob/SlAiFlobObject.h"

#include "ConstructorHelpers.h"
#include "SlAiDataHandle.h"
#include "SlAiTypes.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"


// Sets default values
ASlAiFlobObject::ASlAiFlobObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = (USceneComponent*)BoxCollision;

	//碰撞属性
	BoxCollision->SetCollisionProfileName(FName("FlobProfile"));
	//物理模拟
	BoxCollision->SetSimulatePhysics(true);
	//锁定旋转
	BoxCollision->SetConstraintMode(EDOFMode::Default);
	BoxCollision->GetBodyInstance()->bLockRotation = true;
	//设置大小
	BoxCollision->SetBoxExtent(FVector(15.0f));

	//模型
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticBaseMesh(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	BaseMesh->SetStaticMesh(StaticBaseMesh.Object);

	//碰撞
	BaseMesh->SetCollisionResponseToChannels(ECR_Ignore);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//设置旋转
	BaseMesh->SetRelativeRotation(FRotator(0.f,0.f,90.f));
	BaseMesh->SetRelativeScale3D(FVector(0.3f));

	//动态创建材质
	UMaterialInterface* StaticObjectIconMat = LoadObject<UMaterialInterface>(NULL,TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/FlobIconMat_Inst.FlobIconMat_Inst'"));
	ObjectIconMatDynamic = UMaterialInstanceDynamic::Create(StaticObjectIconMat,nullptr);
}

// Called when the game starts or when spawned
void ASlAiFlobObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlAiFlobObject::RenderTexture()
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	ObjectIconTex = LoadObject<UTexture>(NULL,*ObjectAttr->TexPath);
	ObjectIconMatDynamic->SetTextureParameterValue(FName("ObjectTex"),ObjectIconTex);
	BaseMesh->SetMaterial(0,ObjectIconMatDynamic);
}

// Called every frame
void ASlAiFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlAiFlobObject::CreateFlobObject(int ObjectId)
{
	ObjectIndex = ObjectId;

	//渲染贴图
	RenderTexture();

	//做随机方向的力
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int DirYaw = Stream.RandRange(-180,180);
	FRotator ForceRot = FRotator(0.f,DirYaw,0.f);

	//添加力
	BoxCollision->AddForce((FVector(0.f,0.f,4.f)+ForceRot.Vector())*60000.f);
}

