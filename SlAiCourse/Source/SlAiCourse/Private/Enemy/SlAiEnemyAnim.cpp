// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/SlAiEnemyAnim.h"

#include "ConstructorHelpers.h"
#include "SlAiEnemyCharacter.h"
#include "Animation/AnimMontage.h"

USlAiEnemyAnim::USlAiEnemyAnim()
{
	Speed = 0.f;
	IdleType = 0.f;

	RootBonePos = FVector::Zero();
	RootBoneAlpha = 0.f;
	CurrPlayTime = 0.f;
	StartYPos = 0.f;

	//等待动画资源
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_I(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_I.Enemy_Idle_I'"));
	AnimIdle_I = StaticAnimIdle_I.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_II(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_II.Enemy_Idle_II'"));
	AnimIdle_II = StaticAnimIdle_II.Object;
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimIdle_III(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/MoveGroup/Enemy_Idle_III.Enemy_Idle_III'"));
	AnimIdle_III = StaticAnimIdle_III.Object;

	//攻击动画
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_I(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_I.MonEnemy_Attack_I'"));
	AnimAttack_I = StaticAnimAttack_I.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_II(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_II.MonEnemy_Attack_II'"));
	AnimAttack_II = StaticAnimAttack_II.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_III(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_III.MonEnemy_Attack_III'"));
	AnimAttack_III = StaticAnimAttack_III.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimAttack_IV(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Attack_IV.MonEnemy_Attack_IV'"));
	AnimAttack_IV = StaticAnimAttack_IV.Object;

	//攻击动画文件
	static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimAttackSeq_III(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Attack_III.Enemy_Attack_III'"));
    AnimAttackSeq_III = StaticAnimAttackSeq_III.Object;
    static ConstructorHelpers::FObjectFinder<UAnimSequence> StaticAnimAttackSeq_IV(TEXT("AnimSequence'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/Enemy_Attack_IV.Enemy_Attack_IV'"));
    AnimAttackSeq_IV = StaticAnimAttackSeq_IV.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StaticAnimHurt(TEXT("AnimMontage'/Game/Res/PolygonAdventure/Mannequin/Enemy/Animation/FightGroup/MonEnemy_Hurt.MonEnemy_Hurt'"));
	AnimHurt = StaticAnimHurt.Object;

}

void USlAiEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!SECharacter)
		SECharacter = Cast<ASlAiEnemyCharacter>(TryGetPawnOwner());
	if (!SECharacter)
		return;

	Speed = SECharacter->GetVelocity().Size();
	

	//如果正在播放第三个攻击动作
	if (Montage_IsPlaying(AnimAttack_III))
	{
		CurrPlayTime += DeltaSeconds;
		CurrPlayTime = FMath::Clamp<float>(CurrPlayTime,0.f,AnimAttackSeq_III->GetPlayLength());
		FTransform OutputTransform;
		AnimAttackSeq_III->GetBoneTransform(OutputTransform,FSkeletonPoseBoneIndex(0),CurrPlayTime,true);
		RootBonePos = FVector(OutputTransform.GetLocation().X,StartYPos,OutputTransform.GetLocation().Z);
	}

	if (Montage_IsPlaying(AnimAttack_IV))
	{
		CurrPlayTime += DeltaSeconds;
		CurrPlayTime = FMath::Clamp<float>(CurrPlayTime,0.f,AnimAttackSeq_IV->GetPlayLength());
		FTransform OutputTransform;
		AnimAttackSeq_IV->GetBoneTransform(OutputTransform,FSkeletonPoseBoneIndex(0),CurrPlayTime,true);
		RootBonePos = FVector(OutputTransform.GetLocation().X,StartYPos,OutputTransform.GetLocation().Z);
	}

	//重置权重
	if (!Montage_IsPlaying(AnimAttack_III) && !Montage_IsPlaying(AnimAttack_III) && RootBoneAlpha > 0.f)
	{
		RootBoneAlpha -= DeltaSeconds * 3;
		RootBoneAlpha  = RootBoneAlpha <= 0.f ? 0.f:RootBoneAlpha;
	}
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

float USlAiEnemyAnim::PlayAttackAction(EEnemyAttackType AttackType)
{
	FRandomStream Stream;
	Stream.GenerateNewSeed();
	int SelectIndex = Stream.RandRange(0, 1);
	FTransform OutputTrans;
	switch (AttackType)
	{
	case EEnemyAttackType::EA_Normal:
		//随机一个
		if (SelectIndex == 0)
		{
			Montage_Play(AnimAttack_I);
			return AnimAttack_I->GetPlayLength();
		}
		else
		{
			Montage_Play(AnimAttack_II);
			return AnimAttack_II->GetPlayLength();
		}
		break;
	case EEnemyAttackType::EA_Dash:
		Montage_Play(AnimAttack_III);
		CurrPlayTime = 0.f;
		RootBonePos = FVector::ZeroVector;
		RootBoneAlpha = 1.f;
		AnimAttackSeq_III->GetBoneTransform(OutputTrans, 0, 0.f, true);
		StartYPos = OutputTrans.GetLocation().Y;
		return AnimAttack_III->GetPlayLength();
		break;
	case EEnemyAttackType::EA_Pursuit:
		Montage_Play(AnimAttack_IV);
		CurrPlayTime = 0;
		RootBonePos = FVector::ZeroVector;
		RootBoneAlpha = 1.f;
		AnimAttackSeq_IV->GetBoneTransform(OutputTrans, 0, 0.f, true);
		StartYPos = OutputTrans.GetLocation().Y;
		return AnimAttack_IV->GetPlayLength();
		break;
	}
	return 0.f;
}

float USlAiEnemyAnim::PlayHurtAction()
{
	if (!Montage_IsPlaying(AnimHurt))
	{
		Montage_Play(AnimHurt);
	}
	return AnimHurt->GetPlayLength();
}
