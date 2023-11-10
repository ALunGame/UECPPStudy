// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SlAiEnemyAnim.h"

#include "ConstructorHelpers.h"
#include "SlAiEnemyCharacter.h"

USlAiEnemyAnim::USlAiEnemyAnim()
{
	Speed = 0.f;
	IdleType = 0.f;

	//等待动画资源
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_I(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_I.Enemy_Idle_I'"));
	AnimIdle_I = StaticAnimIdle_I.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_II(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_II.Enemy_Idle_II'"));
	AnimIdle_II = StaticAnimIdle_II.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_III(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_III.Enemy_Idle_III'"));
	AnimIdle_III = StaticAnimIdle_III.Object;
}

void USlAiEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(TryGetPawnOwner());
	if (!SECharacter)
		return;

	Speed = SECharacter->GetVelocity().Size();
}

float USlAiEnemyAnim::SetIdleType(int NewType)
{
	IdleType = FMath::Clamp<float>((float)NewType,0.f, 2.f);
	if (IdleType == 0)
	{
		return AnimIdle_I->GetPlayLength();
	}
	if (IdleType == 1)
	{
		return AnimIdle_II->GetPlayLength();
	}
	if (IdleType == 2)
	{
		return AnimIdle_III->GetPlayLength();
	}
	return AnimIdle_I->GetPlayLength();
}
