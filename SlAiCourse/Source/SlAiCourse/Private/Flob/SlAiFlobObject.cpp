// Fill out your copyright notice in the Description page of Project Settings.


#include "Flob/SlAiFlobObject.h"

#include "ConstructorHelpers.h"
#include "SlAiDataHandle.h"
#include "SlAiPlayerCharacter.h"
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

	if (!GetWorld())
	{
		return;
	}

	FTimerDelegate DetectPlayerDele;
	DetectPlayerDele.BindUObject(this,&ASlAiFlobObject::DetectPlayer);
	//每秒运行一次，延迟三秒执行
	GetWorld()->GetTimerManager().SetTimer(DetectTime,DetectPlayerDele,1.f,true,3.f);

	//注册销毁事件 (10秒自动销毁)
	FTimerDelegate DestroyDele;
	DestroyDele.BindUObject(this,&ASlAiFlobObject::DestroyEvent);
	GetWorld()->GetTimerManager().SetTimer(DestroyTime,DestroyDele,30.f,false);

	SPCharacter = nullptr;
}

void ASlAiFlobObject::RenderTexture()
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectIndex);
	ObjectIconTex = LoadObject<UTexture>(NULL,*ObjectAttr->TexPath);
	ObjectIconMatDynamic->SetTextureParameterValue(FName("ObjectTex"),ObjectIconTex);
	BaseMesh->SetMaterial(0,ObjectIconMatDynamic);
}

void ASlAiFlobObject::DetectPlayer()
{
	if (!GetWorld())
	{
		return;
	}
	
	//保存检测结果
	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams Parameters;
	Parameters.AddIgnoredActor(this);

	//动态球形监测,检测范围是200
	if (GetWorld()->OverlapMultiByObjectType(Overlaps,GetActorLocation(),FQuat::Identity,ObjectQueryParams,FCollisionShape::MakeSphere(200.f),Parameters))
	{
		for (TArray<FOverlapResult>::TIterator It(Overlaps);It; ++It)
		{
			if (Cast<ASlAiPlayerCharacter>(It->GetActor()))
			{
				SPCharacter = Cast<ASlAiPlayerCharacter>(It->GetActor());
				//判断背包
				if (SPCharacter->IsBagFree(ObjectIndex))
				{
					//添加物品到背包
					SPCharacter->AddObject(ObjectIndex);
					
					//停止检测
					GetWorld()->GetTimerManager().PauseTimer(DetectTime);
					GetWorld()->GetTimerManager().PauseTimer(DestroyTime);

					//关闭物理
					BoxCollision->SetSimulatePhysics(false);
				}
			}
		}
	}
}

void ASlAiFlobObject::DestroyEvent()
{
	if (!GetWorld())
	{
		return;
	}

	//注销定时器
	GetWorld()->GetTimerManager().ClearTimer(DetectTime);
	GetWorld()->GetTimerManager().ClearTimer(DestroyTime);

	//销毁自己
	GetWorld()->DestroyActor(this);
}

// Called every frame
void ASlAiFlobObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//一直旋转
	BaseMesh->AddLocalRotation(FRotator(DeltaTime*60.f,0.f,0.f));

	//如果有玩家
	if (SPCharacter)
	{
		//靠近玩家
		SetActorLocation(FMath::VInterpTo(GetActorLocation(),SPCharacter->GetActorLocation()+FVector(0.f,0.f,40.f),DeltaTime,5.f));

		//距离接近
		if (FVector::Distance(GetActorLocation(),SPCharacter->GetActorLocation()+FVector(0.f,0.f,40.f))< 10.f)
		{
			//判断背包
			if (SPCharacter->IsBagFree(ObjectIndex))
			{
				//添加物品到背包
				SPCharacter->AddObject(ObjectIndex);
				//销毁自己
				DestroyEvent();
			}
			else
			{
				//重置
				SPCharacter = nullptr;
				//开启检测
				GetWorld()->GetTimerManager().UnPauseTimer(DetectTime);
				GetWorld()->GetTimerManager().UnPauseTimer(DestroyTime);
				//开启物理
				BoxCollision->SetSimulatePhysics(true);
			}
		}
	}
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
	BoxCollision->AddForce((FVector(0.f,0.f,4.f)+ForceRot.Vector())*100000.f);
}

void ASlAiFlobObject::ThrowFlobObject(int ObjectId, float DirYaw)
{
	ObjectIndex = ObjectId;

	//渲染贴图
	RenderTexture();

	//做随机方向的力
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	DirYaw += Stream.RandRange(-30,30);

	FRotator ForceRot = FRotator(0.f,DirYaw,0.f);

	//添加力
	BoxCollision->AddForce((FVector(0.f,0.f,2.f)+ForceRot.Vector())*120000.f);
}

