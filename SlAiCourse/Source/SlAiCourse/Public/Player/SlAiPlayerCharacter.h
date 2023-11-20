// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "GameFramework/Character.h"
#include "SlAiPlayerCharacter.generated.h"

class ASlAiPlayerController;

UCLASS()
class SLAICOURSE_API ASlAiPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
	class USpringArmComponent* CameraBoon;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
	class UCameraComponent* ThirdCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
	UCameraComponent* FirstCamera;

	/*
	 * 上半身动画状态
	 */
	EUpperBodyAnim::Type UpperBodyAnim;

	/*
	 * 运行切换视角
	 */
	bool IsAllowSwitchView;

	/*
	 * 当前控制器
	 */
	ASlAiPlayerController* MineController;

	//是否在攻击
	bool IsAttack;
	
public:
	// Sets default values for this character's properties
	ASlAiPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	 * 改变视野
	 */
	void ChangeView(EViewType::Type ViewType);

	/*
	 * 修改当前手持物品
	 */
	void ChangeHandObject(TSubclassOf<AActor> HandObjectClass);

	/*
	* 修改物品碰撞检测
	*/
	void ChangeHandObjectDetect(bool IsOpen);

	/*
	* 修改物品的显示
	*/
	void ChangeHandObjectRender(bool IsOpen);

	void PlayerThrowObject(int ObjectID, int Num);

	bool IsBagFree(int ObjectID);

	void AddObject(int ObjectID);

	void EatUpEvent();

	//是否死亡
	bool IsPlayerDead();

	void AcceptDamage(int DamageVal);

	FVector GetCameraPos();

	float PlayDeadAnim();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		USkeletalMeshComponent* MeshFirst;

	UPROPERTY(VisibleDefaultsOnly, Category = "SlAi")
		UChildActorComponent* HandObject;

	class UAnimationAsset* AnimDead;
};
