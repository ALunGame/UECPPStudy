// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "GameFramework/Character.h"
#include "SlAiEnemyCharacter.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	//资源iD
	int ResourceIndex;
	bool IsDestroyNextTick;

public:
	// Sets default values for this character's properties
	ASlAiEnemyCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//实时更新血条方向
	void UpdateHPBarRotation(FVector SPLocation);

	//设置速度
	void SetMaxSpeed(float Speed);

	//获取等待时常
	float GetIdleWaitTime();

	//攻击动画
	float PlayAttackAction(EEnemyAttackType AttackType);

	//接受攻击。也可以重写，TakeDamage
	void AcceptDamage(int DamageValue);

	//播放受伤动画
	float PlayHurtAction();
	
	//开始防御
	void StartDefence();

	//开始防御
	void StopDefence();

	void DestroyEvent();

	FText GetInfoText() const;

	void ChangeWeaponDetect(bool IsOpen);

	bool IsLockPlayer();
	void LoadHP(float Element);
	float GetHP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateFlobObjects();
	
protected:

	//武器插槽
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* WeaponSocket;

	//盾牌插槽
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UChildActorComponent* SheildSocket;
	

	//血条
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UWidgetComponent* HPBar;

	//敌人感知
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UPawnSensingComponent* EnemySense;

private:

	//血条
	TSharedPtr<class SlAIEnemyHPWidget> HPBarWidget;
	
	class ASlAiEnemyController* SEController;

	float HP;

	class USlAiEnemyAnim* SEAnim;

	//死亡动画资源
	UAnimationAsset* AnimDead_I;
	UAnimationAsset* AnimDead_II;

	//死亡销毁时间
	FTimerHandle DeadHandle;
private:

	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);
};
