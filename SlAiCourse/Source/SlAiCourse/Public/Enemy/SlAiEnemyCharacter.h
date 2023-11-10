// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SlAiEnemyCharacter.generated.h"

UCLASS()
class SLAICOURSE_API ASlAiEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

private:

	UFUNCTION()
		void OnSeePlayer(APawn* PlayerChar);
};
