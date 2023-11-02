// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiThirdPlayerAnim.h"

#include "ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"

USlAiThirdPlayerAnim::USlAiThirdPlayerAnim()
{
	Direction = 0.f;
	IsInAir = false;

	//加载蒙太奇动画
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerHitMontage.PlayerHitMontage'"));
	MontageMap.Add(EUpperBodyAnim::Hit,PlayerHitMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerFightMontage.PlayerFightMontage'"));
	MontageMap.Add(EUpperBodyAnim::Fight,PlayerFightMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPunchMontage.PlayerPunchMontage'"));
	MontageMap.Add(EUpperBodyAnim::Punch,PlayerPunchMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerEatMontage.PlayerEatMontage'"));
	MontageMap.Add(EUpperBodyAnim::Eat,PlayerEatMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Player/Animation/UpperBody/PlayerPickUpMontage.PlayerPickUpMontage'"));
	MontageMap.Add(EUpperBodyAnim::PickUp,PlayerPickUpMon.Object);

	ViewType = EViewType::Third;
}

void USlAiThirdPlayerAnim::UpdateParam()
{
	Super::UpdateParam();

	//参数
	IsInAir = SPCharacter->GetCharacterMovement()->IsFalling();

	//方向

	//速度方向-当前朝向
	float PreDir = SPCharacter->GetVelocity().ToOrientationRotator().Yaw - SPCharacter->GetActorRotation().Yaw;

	//TODO 
	if (PreDir > 180.f)
	{
		PreDir -= 360.f;
	}
	if (PreDir < -180.f)
	{
		PreDir += 360.f;
	}

	Direction = PreDir;
}
