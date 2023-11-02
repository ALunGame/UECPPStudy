// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiPlayerCharacter.h"

#include "ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "Hand/SlAiHandObject.h"


// Sets default values
ASlAiPlayerCharacter::ASlAiPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//自动接受输入
	AutoReceiveInput = EAutoReceiveInput::Player0;

	//设置碰撞配置
	GetCapsuleComponent()->SetCollisionProfileName(FName("PlayerProfile"));

	//添加第一人称模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshFirst(TEXT("/Script/Engine.SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/SkMesh/FirstPlayer.FirstPlayer'"));
	MeshFirst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFirst"));
	MeshFirst->SetSkeletalMesh(StaticMeshFirst.Object);
	//附加在CapsuleComponent上
	MeshFirst->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	MeshFirst->bCastDynamicShadow = false;
	MeshFirst->bOnlyOwnerSee = true;
	MeshFirst->bReceivesDecals = false;
	//更新频率
	MeshFirst->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	MeshFirst->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	//碰撞属性
	MeshFirst->SetCollisionObjectType(ECC_Pawn);
	MeshFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshFirst->SetCollisionResponseToAllChannels(ECR_Ignore);
	//位置朝向
	MeshFirst->SetRelativeLocation(FVector(0.f,0.f,-95.f));
	MeshFirst->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f,0.f,-90.f)));

	//第一人称动画蓝图
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimFirst(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Player/BP_FirstPlayerAnim.BP_FirstPlayerAnim_C'"));
	MeshFirst->AnimClass = StaticAnimFirst.Class;
	
	//添加第三人称模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticMeshThird(TEXT("/Script/Engine.SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Player/SkMesh/Player.Player'"));
	GetMesh()->SetSkeletalMesh(StaticMeshThird.Object);
	GetMesh()->bOnlyOwnerSee = true;
	GetMesh()->bReceivesDecals = false;
	//碰撞属性
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//位置朝向
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-95.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f,0.f,-90.f)));

	//第三人称动画蓝图
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticAnimThird(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Player/BP_ThirdPlayerAnim.BP_ThirdPlayerAnim_C'"));
	GetMesh()->AnimClass = StaticAnimThird.Class;

	//摄像机手臂
	CameraBoon = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoon"));
	CameraBoon->SetupAttachment(RootComponent);
	//设置距离
	CameraBoon->TargetArmLength = 300.f;
	CameraBoon->TargetOffset = FVector(0.f,0.f,60.f);
	//绑定Controller的旋转
	CameraBoon->bUsePawnControlRotation = true;

	//第三人称相机
	ThirdCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCamera"));
	ThirdCamera->SetupAttachment(CameraBoon,USpringArmComponent::SocketName);
	//设置不跟随
	ThirdCamera->bUsePawnControlRotation = false;

	//第一人称相机
	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetupAttachment((USceneComponent*)GetCapsuleComponent());
	//设置跟随
	FirstCamera->bUsePawnControlRotation = true;
	//设置在头部位置
	FirstCamera->AddLocalOffset(FVector(0.f,0.f,60.f));

	//默认动画状态
	UpperBodyAnim = EUpperBodyAnim::None;

	//实例化手上物品
	HandObject = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandObject"));

	IsAllowSwitchView = false;
}

// Called when the game starts or when spawned
void ASlAiPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//手持物品绑定到插槽上
	HandObject->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("RHSocket"));

	//添加Actor到HandObject
	HandObject->SetChildActorClass(ASlAiHandObject::SpawnHandObject(0));
}

// Called every frame
void ASlAiPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASlAiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASlAiPlayerCharacter::ChangeView(EViewType::Type ViewType)
{
	if (ViewType == EViewType::First)
	{
		//设置默认相机
		FirstCamera->SetActive(true);
		ThirdCamera->SetActive(false);

		//设置模型
		GetMesh()->SetOwnerNoSee(true);
		MeshFirst->SetOwnerNoSee(false);

		//修改手持绑定位置
		HandObject->AttachToComponent(MeshFirst,FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("RHSocket"));
	}
	else if (ViewType == EViewType::Third)
	{
		//设置默认相机
		FirstCamera->SetActive(false);
		ThirdCamera->SetActive(true);

		//设置模型
		GetMesh()->SetOwnerNoSee(false);
		MeshFirst->SetOwnerNoSee(true);

		//修改手持绑定位置
		HandObject->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("RHSocket"));
	}
}

void ASlAiPlayerCharacter::ChangeHandObject(TSubclassOf<AActor> HandObjectClass = nullptr)
{
	if (!HandObjectClass)
	{
		HandObject->DestroyChildActor();
		return;
	}

	//设置物品
	HandObject->SetChildActorClass(HandObjectClass);
}

void ASlAiPlayerCharacter::ChangeHandObjectDetect(bool IsOpen)
{
	ASlAiHandObject* HandObjectClass = Cast<ASlAiHandObject>(HandObject->GetChildActor());
	if (HandObjectClass)
	{
		HandObjectClass->ChangeOverlayDetect(IsOpen);
	}
}

void ASlAiPlayerCharacter::ChangeHandObjectRender(bool IsOpen)
{
	if (!HandObject)
	{
		return;
	}
	HandObject->GetChildActor()->SetActorHiddenInGame(!IsOpen);
}
