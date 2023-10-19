// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Player\SlAiPlayerController.h"

#include "ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SlAiHelper.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ASlAiPlayerController::ASlAiPlayerController()
{
	LoadInputMoveMapping();
	LoadInputInteractiveMapping();

	//速率初始化
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//初始化视野
	CurrViewType = EViewType::Third;

	//初始化
	LeftMouseClickAnim = EUpperBodyAnim::Eat;
	RightMouseClickAnim = EUpperBodyAnim::Hit;
}

void ASlAiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMoveContext, 100);
		Subsystem->AddMappingContext(InputInteractiveMapping, 100);
	}
	
	RegisterInputMove();
	RegisterInputInteractive();
}

void ASlAiPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<ASlAiPlayerCharacter>(InPawn);
	InitPlayerCharacter();
}

void ASlAiPlayerController::BeginPlay()
{
	//鼠标不显示
	bShowMouseCursor = false;

	//输入模式
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);
	SetInputMode(InputMode);
}

void ASlAiPlayerController::InitPlayerCharacter()
{
	PlayerCharacter->ChangeView(CurrViewType);
	PlayerCharacter->MineController = this;
}

#pragma region 移动实现

void ASlAiPlayerController::LoadInputMoveMapping()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMapping(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Move.IMC_Move'"));
	InputMoveContext = InputMapping.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_MoveForward(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveForward.IA_MoveForward'"));
	IA_MoveForward = Input_MoveForward.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_MoveRight(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_MoveRight.IA_MoveRight'"));
	IA_MoveRight = Input_MoveRight.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_Turn(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Turn.IA_Turn'"));
	IA_Turn = Input_Turn.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_LookUp(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_LookUp.IA_LookUp'"));
	IA_LookUp = Input_LookUp.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_TurnRate(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_TurnRate.IA_TurnRate'"));
	IA_TurnRate = Input_TurnRate.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	IA_Jump = Input_Jump.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_Run(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Run.IA_Run'"));
	IA_Run = Input_Run.Object;
}

void ASlAiPlayerController::RegisterInputMove()
{
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if(IA_MoveForward)
		{
			EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &ASlAiPlayerController::MoveForward);
			SlAiHelper::Debug(FString("IA_MoveForward>>>"),60.f);
		}
	
		if(IA_MoveRight)
		{
			EnhancedInputComponent->BindAction(IA_MoveRight, ETriggerEvent::Triggered, this, &ASlAiPlayerController::MoveRight);
			SlAiHelper::Debug(FString("IA_MoveRight>>>"),60.f);
		}

		if(IA_Turn)
		{
			EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ASlAiPlayerController::Turn);
			SlAiHelper::Debug(FString("IA_Turn>>>"),60.f);
		}
		
		if(IA_LookUp)
		{
			EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ASlAiPlayerController::LookUpAtRate);
			SlAiHelper::Debug(FString("IA_LookUp>>>"),60.f);
		}

		if(IA_TurnRate)
		{
			EnhancedInputComponent->BindAction(IA_TurnRate, ETriggerEvent::Triggered, this, &ASlAiPlayerController::TurnAtRate);
			SlAiHelper::Debug(FString("IA_TurnRate>>>"),60.f);
		}

		if(IA_Jump)
		{
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASlAiPlayerController::OnStartJump);
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ASlAiPlayerController::OnStopJump);
			SlAiHelper::Debug(FString("IA_Jump>>>"),60.f);
		}

		if(IA_Run)
		{
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, this, &ASlAiPlayerController::OnStartRun);
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, this, &ASlAiPlayerController::OnStopRun);
			SlAiHelper::Debug(FString("IA_Run>>>"),60.f);
		}
	}
}

void ASlAiPlayerController::MoveForward(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr) && Value.IsNonZero())
	{
		//找到当前前方方向
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//指定向量位移
		GetPawn()->AddMovementInput(Direction, Value.GetMagnitude());
	}
}

void ASlAiPlayerController::MoveRight(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr) && (Value.IsNonZero()))
	{
		//找到当前右方向
		const FRotator Rotation = GetControlRotation();;
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//向量
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//指定向量位移
		GetPawn()->AddMovementInput(Direction, Value.GetMagnitude());
	}
}

void ASlAiPlayerController::LookUpAtRate(const FInputActionValue& Value)
{
	AddPitchInput(Value.GetMagnitude() * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerController::Turn(const FInputActionValue& Value)
{
	AddYawInput(Value.GetMagnitude());
}

void ASlAiPlayerController::TurnAtRate(const FInputActionValue& Value)
{
	AddYawInput(Value.GetMagnitude() * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASlAiPlayerController::OnStartJump(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr))
	{
		Cast<ACharacter>(GetPawn())->bPressedJump = true;
	}
}

void ASlAiPlayerController::OnStopJump(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr))
	{
		Cast<ACharacter>(GetPawn())->bPressedJump = false;
		Cast<ACharacter>(GetPawn())->StopJumping();
	}
}

void ASlAiPlayerController::OnStartRun(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr))
	{
		Cast<UCharacterMovementComponent>(GetPawn()->GetMovementComponent())->MaxWalkSpeed = 375.f;
	}
}

void ASlAiPlayerController::OnStopRun(const FInputActionValue& Value)
{
	if ((GetPawn() != nullptr))
	{
		Cast<UCharacterMovementComponent>(GetPawn()->GetMovementComponent())->MaxWalkSpeed = 150.f;
	}
}

#pragma endregion

#pragma region 交互实现

void ASlAiPlayerController::LoadInputInteractiveMapping()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMapping(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Interactive.IMC_Interactive'"));
	InputInteractiveMapping = InputMapping.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_ChangeView(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_ChangeView.IA_ChangeView'"));
	IA_ChangeView = Input_ChangeView.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_LeftMouse(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_LeftMouseClick.IA_LeftMouseClick'"));
	IA_LeftMouseClick = Input_LeftMouse.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> Input_RightMouse(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_RightMouseClick.IA_RightMouseClick'"));
	IA_RightMouseClick = Input_RightMouse.Object;
}

void ASlAiPlayerController::RegisterInputInteractive()
{
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		if(IA_ChangeView)
		{
			EnhancedInputComponent->BindAction(IA_ChangeView, ETriggerEvent::Started, this, &ASlAiPlayerController::OnChangeView);
			SlAiHelper::Debug(FString("IA_ChangeView>>>"),60.f);
		}

		if(IA_LeftMouseClick)
		{
			EnhancedInputComponent->BindAction(IA_LeftMouseClick, ETriggerEvent::Started, this, &ASlAiPlayerController::OnLeftMouseClickStart);
			EnhancedInputComponent->BindAction(IA_LeftMouseClick, ETriggerEvent::Completed, this, &ASlAiPlayerController::OnLeftMouseClickEnd);
			SlAiHelper::Debug(FString("IA_LeftMouseClick>>>"),60.f);
		}

		if(IA_RightMouseClick)
		{
			EnhancedInputComponent->BindAction(IA_RightMouseClick, ETriggerEvent::Started, this, &ASlAiPlayerController::OnRightMouseClickStart);
			EnhancedInputComponent->BindAction(IA_RightMouseClick, ETriggerEvent::Completed, this, &ASlAiPlayerController::OnRightMouseClickEnd);
			SlAiHelper::Debug(FString("IA_RightMouseClick>>>"),60.f);
		}
	}
}

void ASlAiPlayerController::OnChangeView(const FInputActionValue& Value)
{
	if (!PlayerCharacter->IsAllowSwitchView)
	{
		return;
	}
	
	EViewType::Type NewViewType = EViewType::Third;
	switch (CurrViewType) {
	case EViewType::First:
		NewViewType = EViewType::Third;
		break;
	case EViewType::Third:
		NewViewType = EViewType::First;
		break;
	default: ;
	}

	CurrViewType = NewViewType;
	PlayerCharacter->ChangeView(CurrViewType);
}

void ASlAiPlayerController::OnLeftMouseClickStart(const FInputActionValue& Value)
{
	PlayerCharacter->UpperBodyAnim = LeftMouseClickAnim;
	SlAiHelper::DebugWarning("OnLeftMouseClickStart");
}

void ASlAiPlayerController::OnLeftMouseClickEnd(const FInputActionValue& Value)
{
	PlayerCharacter->UpperBodyAnim = EUpperBodyAnim::None;
	SlAiHelper::DebugWarning("OnLeftMouseClick____End");
}

void ASlAiPlayerController::OnRightMouseClickStart(const FInputActionValue& Value)
{
	PlayerCharacter->UpperBodyAnim = RightMouseClickAnim;
}

void ASlAiPlayerController::OnRightMouseClickEnd(const FInputActionValue& Value)
{
	PlayerCharacter->UpperBodyAnim = EUpperBodyAnim::None;
}


#pragma endregion






