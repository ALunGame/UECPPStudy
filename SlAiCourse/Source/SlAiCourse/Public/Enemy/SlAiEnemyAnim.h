// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "Animation/AnimInstance.h"
#include "SlAiEnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	USlAiEnemyAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	float SetIdleType(int NewType);

	float PlayAttackAction(EEnemyAttackType AttackType);

	//播放受伤动画
	float PlayHurtAction();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float IdleType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		FVector RootBonePos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyAnim)
		float RootBoneAlpha;
	
protected:

	class ASlAiEnemyCharacter* SECharacter;

	//等待动作
	UAnimSequence* AnimIdle_I;
	UAnimSequence* AnimIdle_II;
	UAnimSequence* AnimIdle_III;

	//攻击动画
	UAnimMontage* AnimAttack_I;
	UAnimMontage* AnimAttack_II;
	UAnimMontage* AnimAttack_III;
	UAnimMontage* AnimAttack_IV;

	UAnimSequence* AnimAttackSeq_III;
	UAnimSequence* AnimAttackSeq_IV;

	//受伤
	UAnimMontage* AnimHurt;

	//动作计时器
	float CurrPlayTime;
	//动作第一帧Y位置
	float StartYPos;
	
};
