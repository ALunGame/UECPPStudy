// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SlAiEnemyCharacter.h"

#include "ConstructorHelpers.h"
#include "SlAiDataHandle.h"
#include "SlAiEnemyAnim.h"
#include "SlAiEnemyController.h"
#include "SlAIEnemyHPWidget.h"
#include "SlAiEnemyTool.h"
#include "SlAiFlobObject.h"
#include "SlAiPlayerCharacter.h"
#include "WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Classes/Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ASlAiEnemyCharacter::ASlAiEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//设置AI控制器
	AIControllerClass = ASlAiEnemyController::StaticClass();

	//设置碰撞
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyProfile"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	//添加模型
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> StaticEnemyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Res/PolygonAdventure/Mannequin/Enemy/SkMesh/Enemy.Enemy'"));
	GetMesh()->SetSkeletalMesh(StaticEnemyMesh.Object);
	//设置碰撞
	GetMesh()->SetCollisionObjectType(ECC_Pawn);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//模型位置
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-95.f));
	GetMesh()->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.f,0.f,-90.f)));

	//添加动画蓝图
	static ConstructorHelpers::FClassFinder<UAnimInstance> StaticEnemyAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprint/Enemy/Enemy_Anim.Enemy_Anim_C'"));
	GetMesh()->AnimClass = StaticEnemyAnim.Class;

	//创建插槽
	WeaponSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSocket"));
	SheildSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("SheildSocket"));

	//实列化血条
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar"));
	HPBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//实例化敌人感知
	EnemySense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemySense"));

	//死亡动画
	AnimDead_I = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_I.Enemy_Dead_I'")));
	AnimDead_II = Cast<UAnimationAsset>(StaticLoadObject(UAnimationAsset::StaticClass(), NULL, *FString("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Dead_II.Enemy_Dead_II'")));

	IsDestroyNextTick = false;
}

// Called when the game starts or when spawned
void ASlAiEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//获取动画
	SEAnim = Cast<USlAiEnemyAnim>(GetMesh()->GetAnimInstance());
	SEController = Cast<ASlAiEnemyController>(GetController());

	//绑定插槽
	WeaponSocket->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("RHSocket"));
	SheildSocket->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,FName("LHSocket"));

	//给插槽添加物品
	WeaponSocket->SetChildActorClass(ASlAiEnemyTool::SpawnEnemyWeapon());
	SheildSocket->SetChildActorClass(ASlAiEnemyTool::SpawnEnemySheild());

	//设置血条
	SAssignNew(HPBarWidget,SlAIEnemyHPWidget);
	HPBar->SetSlateWidget(HPBarWidget);
	HPBar->SetRelativeLocation(FVector(0,0,100.f));
	HPBar->SetDrawSize(FVector2d(100.f,10.f));
	//初始化血量
	HP = 200.f;
	HPBarWidget->ChangeHP(HP / 200.f);

	//敌人感知
	EnemySense->HearingThreshold = 0.f;
	EnemySense->LOSHearingThreshold = 0.f;
	EnemySense->SightRadius = 1000.f;
	EnemySense->SetPeripheralVisionAngle(55.f);
	EnemySense->bHearNoises = false;
	//绑定方法
	FScriptDelegate OnSeePlayerDele;
	OnSeePlayerDele.BindUFunction(this, "OnSeePlayer");
	EnemySense->OnSeePawn.Add(OnSeePlayerDele);
	//EnemySense->OnSeePawn.AddDynamic(this, &ASlAiEnemyCharacter::OnSeePlayer);

	ResourceIndex = 3;
}

void ASlAiEnemyCharacter::CreateFlobObjects()
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);

	for (TArray<TArray<int>>::TIterator It(ResourceAttr->FlobObjectInfo); It; ++It)
	{
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int Num = Stream.RandRange((*It)[1], (*It)[2]);

		if (GetWorld())
		{
			for (int i = 0; i < Num; ++i)
			{
				ASlAiFlobObject* FlobObject = GetWorld()->SpawnActor<ASlAiFlobObject>(GetActorLocation() + FVector(0.f, 0.f, 40.f), FRotator::ZeroRotator);
				FlobObject->CreateFlobObject((*It)[0]);
			}
		}
	}
}

void ASlAiEnemyCharacter::OnSeePlayer(APawn* PlayerChar)
{
	if (Cast<ASlAiPlayerCharacter>(PlayerChar))
	{
		SlAiHelper::Debug("Enemy See Player",3.f);
		if (SEController)
		{
			SEController->OnSeePlayer();
		}
	}
}

// Called every frame
void ASlAiEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDestroyNextTick)
	{
		DestroyEvent();
		IsDestroyNextTick = false;
	}
}

// Called to bind functionality to input
void ASlAiEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASlAiEnemyCharacter::UpdateHPBarRotation(FVector SPLocation)
{
	FVector StartPos(GetActorLocation().X,GetActorLocation().Y,0);
	FVector TargetPos(SPLocation.X,SPLocation.Y,0);
	HPBar->SetWorldRotation(FRotationMatrix::MakeFromX(TargetPos-StartPos).Rotator());
}

void ASlAiEnemyCharacter::SetMaxSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

float ASlAiEnemyCharacter::GetIdleWaitTime()
{
	if (!SEAnim)
	{
		return 3.f;
	}

	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int IdleType = Stream.RandRange(0,2);

	float AnimLength = SEAnim->SetIdleType(IdleType);
	Stream.GenerateNewSeed();

	int AnimCnt = Stream.RandRange(1,3);
	return AnimCnt * AnimLength;
}

float ASlAiEnemyCharacter::PlayAttackAction(EEnemyAttackType AttackType)
{
	if (!SEAnim)
	{
		return 0.f;
	}

	return SEAnim->PlayAttackAction(AttackType);
}

void ASlAiEnemyCharacter::AcceptDamage(int DamageValue)
{
	if (SEAnim && SEAnim->IsDefence)
	{
		return;
	}
	
	HP = FMath::Clamp<float>(HP - DamageValue, 0.f, 200.f);
	HPBarWidget->ChangeHP(HP / 200.f);
	if (HP == 0.f && !DeadHandle.IsValid())
	{
		SEController->EnemyDead();
		SEAnim->StopAllAction();

		float DeadDuration = 0.f;
		FRandomStream Stream;
		Stream.GenerateNewSeed();
		int SelectIndex = Stream.RandRange(0,1);
		if (SelectIndex == 0)
		{
			GetMesh()->PlayAnimation(AnimDead_I,false);
			DeadDuration = AnimDead_I->GetPlayLength()*2;
		}
		else
		{
			GetMesh()->PlayAnimation(AnimDead_II,false);
			DeadDuration = AnimDead_I->GetPlayLength()*2;
		}

		//生成掉落物
		CreateFlobObjects();
		
		//时间委托
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASlAiEnemyCharacter::DestroyEvent);
		GetWorld()->GetTimerManager().SetTimer(DeadHandle, TimerDelegate, DeadDuration, false);
	}
	else
	{
		if (SEController) SEController->UpdateDamageRatio(HP / 200.f);
	}
}

float ASlAiEnemyCharacter::PlayHurtAction()
{
	if (!SEAnim)
	{
		return 0.f;
	}

	return SEAnim->PlayHurtAction();
}

void ASlAiEnemyCharacter::StartDefence()
{
	if (SEAnim)
	{
		SEAnim->IsDefence = true;
	}
}

void ASlAiEnemyCharacter::StopDefence()
{
	if (SEAnim)
	{
		SEAnim->IsDefence = false;
	}
}

void ASlAiEnemyCharacter::DestroyEvent()
{
	//销毁时间函数
	if (DeadHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DeadHandle);
	}
	GetWorld()->DestroyActor(this);
}

FText ASlAiEnemyCharacter::GetInfoText() const
{
	TSharedPtr<ResourceAttribute> ResourceAttr = *SlAiDataHandle::Get()->ResourceAttrMap.Find(ResourceIndex);

	switch (SlAiDataHandle::Get()->CurCulture)
	{
	case ECultureTeam::EN:
		return ResourceAttr->EN;
	case ECultureTeam::ZH:
		return ResourceAttr->ZH;
	default: ;
	}
	return ResourceAttr->ZH;
}

void ASlAiEnemyCharacter::ChangeWeaponDetect(bool IsOpen)
{
	ASlAiEnemyTool* WeaponClass = Cast<ASlAiEnemyTool>(WeaponSocket->GetChildActor());
	if (WeaponClass)
	{
		WeaponClass->ChangeOverlayDetect(IsOpen);
	}
}

bool ASlAiEnemyCharacter::IsLockPlayer()
{
	return SEController->IsLockPlayer;
}

void ASlAiEnemyCharacter::LoadHP(float Element)
{
	HP = Element;
	HPBarWidget->ChangeHP(HP / 200.f);
}

float ASlAiEnemyCharacter::GetHP()
{
	return HP;
}

