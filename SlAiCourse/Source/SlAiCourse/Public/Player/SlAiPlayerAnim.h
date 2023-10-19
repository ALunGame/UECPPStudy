// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiPlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "SlAiTypes.h"
#include "SlAiPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	USlAiPlayerAnim();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		float Speed;

	//上半身旋转
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		FRotator SpineRotator;

	/*
	 * 视角动画
	 */
	EViewType::Type ViewType;

protected:

	void InitSPCharacter();

	//更新动画参数
	virtual void UpdateParam();

	/*
	* 更新动画蒙太奇
	*/
	virtual void UpdateMontage();

	/*
	* 允许切换视角
	*/
	virtual void AllowChangeView(bool IsAllow);

protected:

	ASlAiPlayerCharacter* SPCharacter;

#pragma region 蒙太奇动画

	/*
	 * 当前蒙太奇动画 
	 */
	UAnimMontage* CurrentMontage;

	/*
	* 动画映射 
	*/
	TMap<EUpperBodyAnim::Type,UAnimMontage*> MontageMap;
	
#pragma endregion
	
};
