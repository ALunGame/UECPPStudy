// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"

#include "EngineUtils.h"
#include "SlAiBagManager.h"
#include "SlAiDataHandle.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiGameHUD.h"
#include "SlAiGameInstance.h"
#include "SlAiHelper.h"
#include "SlAiSceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SlAiPlayerCharacter.h"
#include "..\..\Public\Player\SlAiPlayerController.h"
#include "Player/SlAiPlayerState.h"

ASlAiGameMode::ASlAiGameMode()
{
	//允许开始Tick函数
	PrimaryActorTick.bCanEverTick = true;
	
	//绑定自定义类
	HUDClass = ASlAiGameHUD::StaticClass();
	
	PlayerControllerClass = ASlAiPlayerController::StaticClass();
	PlayerStateClass = ASlAiPlayerState::StaticClass();

	// PlayerControllerClass = ASlAiPlayerController::StaticClass();
	// PlayerStateClass = ASlAiPlayerState::StaticClass();
	
	DefaultPawnClass = ASlAiPlayerCharacter::StaticClass();

	IsInitBag = false;

	IsInitMiniMap = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	
	InitMiniMapCamera();
	InitBagManager();
}

void ASlAiGameMode::InitGameplayMode()
{
	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	SPCharacter = Cast<ASlAiPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	SPState = Cast<ASlAiPlayerState>(SPController->PlayerState);
	//SPState = Cast<ASlAiPlayerState>(SPController.state);
}

void ASlAiGameMode::BeginPlay()
{
	//SlAiHelper::DebugWarning(FString("DataHandle:" + SlAiDataHandle::Get()->RecordName),30.f);
	//SlAiHelper::DebugWarning(FString("GameInstance:" + Cast<USlAiGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GameName),30.f);

	SlAiDataHandle::Get()->InitGameData();

	SlAiHelper::DebugWarning(FString("InitGameData:" + SlAiDataHandle::Get()->RecordName),30.f);
	
	if (!SPController)
	{
		InitGameplayMode();
	}
}

void ASlAiGameMode::InitBagManager()
{
	if (IsInitBag)
	{
		return;
	}
	IsInitBag = true;

	//执行委托
	InitBagManger.ExecuteIfBound();

	//绑定委托
	SlAiBagManager::Get()->PlayerThrowObject.BindUObject(SPCharacter,&ASlAiPlayerCharacter::PlayerThrowObject);
	SlAiBagManager::Get()->ChangeHandObject.BindUObject(SPState,&ASlAiPlayerState::ChangeHandObject);
}

void ASlAiGameMode::InitMiniMapCamera()
{
	if (!IsInitMiniMap && GetWorld())
	{
		MiniMapCamera = GetWorld()->SpawnActor<ASlAiSceneCapture2D>(ASlAiSceneCapture2D::StaticClass());
		RegisterMiniMap.ExecuteIfBound(MiniMapCamera->GetMiniMapTex());
		SPController->UpdateMiniMapWidth.BindUObject(MiniMapCamera,&ASlAiSceneCapture2D::UpdateMiniMapWidth);
		IsInitMiniMap = true;
	}

	if (IsInitMiniMap)
	{
		MiniMapCamera->UpdateTransform(SPCharacter->GetActorLocation(),SPCharacter->GetActorRotation());

		TArray<FVector2D> EnemyPosList;
		TArray<bool> EnemyLockList;
		TArray<float> EnemyRotateList;

		FVector PlayerPos = SPCharacter->GetActorLocation();
		//获取场景中的敌人
		for	(TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
		{
			FVector EnemyPos = FVector((*EnemyIt)->GetActorLocation().X - SPCharacter->GetActorLocation().X, (*EnemyIt)->GetActorLocation().Y - SPCharacter->GetActorLocation().Y, 0.f);
			EnemyPos = FQuat(FVector::UpVector, FMath::DegreesToRadians(-SPCharacter->GetActorRotation().Yaw - 90.f)) * EnemyPos;
			EnemyPosList.Add(FVector2D(EnemyPos.X, EnemyPos.Y));

			EnemyLockList.Add((*EnemyIt)->IsLockPlayer());
			EnemyRotateList.Add((*EnemyIt)->GetActorRotation().Yaw - SPCharacter->GetActorRotation().Yaw);
		}
		

		UpdateMapData.ExecuteIfBound(SPCharacter->GetActorRotation(),MiniMapCamera->GetMapSize(),&EnemyPosList,&EnemyLockList,&EnemyRotateList);
	}
	
}
