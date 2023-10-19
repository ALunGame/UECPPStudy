// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/SlAiPlayerAnim.h"
#include "SlAiThirdPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USlAiThirdPlayerAnim : public USlAiPlayerAnim
{
	GENERATED_BODY()

public:
	USlAiThirdPlayerAnim();

protected:
	virtual void UpdateParam() override;
	
public:

	//是否在空中
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		bool IsInAir;

	//朝向
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PlayAnim)
		float Direction;
};
