// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "SlAiPlayerCharacter.h"
#include "SlAiTypes.h"
#include "GameFramework/PlayerController.h"
#include "SlAiPlayerController.generated.h"

class ASlAiPlayerState;

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/*
	 * 当前视野类型
	 */
	EViewType::Type CurrViewType;

	/*
	 * 玩家
	 */
	ASlAiPlayerCharacter* SPCharacter;

	/*
	 * 状态
	 */
	ASlAiPlayerState* SPState;

	/*
	 * 左键动作
	 */
	EUpperBodyAnim::Type LeftMouseClickAnim;

	/*
	 * 右键动作
	 */
	EUpperBodyAnim::Type RightMouseClickAnim;

	/*
	 * 鼠标左键
	 */
	bool IsMouseLeftDown;

	/*
	 * 鼠标右键
	 */
	bool IsMouseRightDown;

#pragma region 移动输入映射
	
	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move")
	UInputMappingContext* InputMoveContext;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_MoveForward;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_MoveRight;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_Turn;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_LookUp;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_TurnRate;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Move|Action")
	UInputAction* IA_Jump;

	UPROPERTY(VisibleDefaultsOnly, Category="EnhancedInput|Action")
	UInputAction* IA_Run;
	
#pragma endregion

#pragma region 交互输入映射
	
	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive")
	UInputMappingContext* InputInteractiveMapping;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive|Action")
	UInputAction* IA_ChangeView;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive|Action")
	UInputAction* IA_LeftMouseClick;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive|Action")
	UInputAction* IA_RightMouseClick;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive|Action")
	UInputAction* IA_MouseScrollUp;

	UPROPERTY(VisibleDefaultsOnly, Category="Input_Interactive|Action")
	UInputAction* IA_MouseScrollDown;
	
#pragma endregion

public:
	ASlAiPlayerController();
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaSeconds) override;
	
	/*
	 * 修改当前手持物品
	 */
	void ChangeHandObject();

protected:
	virtual void BeginPlay() override;

private:
	
	//旋转上下视角速率
	float BaseLookUpRate;

	//旋转左右视角速率
	float BaseTurnRate;

private:

	void InitPlayerCharacter();

#pragma region 移动声明
	
	void LoadInputMoveMapping();

	void RegisterInputMove();
	
	//前进
	void MoveForward(const FInputActionValue& Value);

	//左右
	void MoveRight(const FInputActionValue& Value);

	//向上旋转
	void LookUpAtRate(const FInputActionValue& Value);

	//转身
	void Turn(const FInputActionValue& Value);

	//左右旋转
	void TurnAtRate(const FInputActionValue& Value);

	//开始跳跃
	void OnStartJump(const FInputActionValue& Value);
	//结束跳跃
	void OnStopJump(const FInputActionValue& Value);

	//开始奔跑
	void OnStartRun(const FInputActionValue& Value);
	//结束奔跑
	void OnStopRun(const FInputActionValue& Value);
	
#pragma endregion

#pragma region 交互声明
	
	void LoadInputInteractiveMapping();

	void RegisterInputInteractive();

	//改变视野
	void OnChangeView(const FInputActionValue& Value);

	//开始鼠标左键
	void OnLeftMouseClickStart(const FInputActionValue& Value);

	//结束鼠标左键
	void OnLeftMouseClickEnd(const FInputActionValue& Value);

	//开始鼠标右键
	void OnRightMouseClickStart(const FInputActionValue& Value);

	//开始鼠标右键
	void OnRightMouseClickEnd(const FInputActionValue& Value);

	//开始鼠标向上滚动
	void OnMouseScrollUp(const FInputActionValue& Value);

	//开始鼠标向下滚动
	void OnMouseScrollDown(const FInputActionValue& Value);
	
#pragma endregion

	//修改动作
	void ChangePreUpperType(EUpperBodyAnim::Type RightType);
	
};
