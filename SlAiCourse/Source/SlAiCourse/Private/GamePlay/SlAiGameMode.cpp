// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiGameMode.h"

#include "EngineUtils.h"
#include "SlAiBagManager.h"
#include "SlAiDataHandle.h"
#include "SlAiEnemyCharacter.h"
#include "SlAiGameHUD.h"
#include "SlAiGameInstance.h"
#include "SlAiHelper.h"
#include "SlAiPickupStone.h"
#include "SlAiPickupWood.h"
#include "SlAiResourceRock.h"
#include "SlAiResourceTree.h"
#include "SlAiSaveGame.h"
#include "SlAiSceneCapture2D.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SlAiPlayerCharacter.h"
#include "..\..\Public\Player\SlAiPlayerController.h"
#include "Player/SlAiPlayerState.h"
#include "Sound/SoundWave.h"

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

	IsNeedLoadRecord = false;
}

void ASlAiGameMode::Tick(float DeltaSeconds)
{
	
	InitMiniMapCamera();

	LoadRecordBag();
	
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

	//播放背景音乐
	USoundWave* BGMusic = LoadObject<USoundWave>(nullptr, TEXT("SoundWave'/Game/Res/Sound/GameSound/GameBG.GameBG'"));
	BGMusic->bLooping = true;
	UGameplayStatics::PlaySound2D(GetWorld(), BGMusic, 0.1f);

	LoadRecord();

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

void ASlAiGameMode::LoadRecord()
{
	if (SlAiDataHandle::Get()->RecordName.IsEmpty() || SlAiDataHandle::Get()->RecordName.Equals(FString("Default"))) 
	{
		return;
	}

	//循环检测是不是有这个存档
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It) {
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName)) {
			IsNeedLoadRecord = true;
			break;
		}
	}

	//加载存档
	if (IsNeedLoadRecord && UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName,0))
	{
		GameRecord = Cast<USlAiSaveGame>(UGameplayStatics::LoadGameFromSlot(SlAiDataHandle::Get()->RecordName,0));
	}
	else
	{
		IsNeedLoadRecord = false;
	}

	if (IsNeedLoadRecord && GameRecord)
	{
		SPCharacter->SetActorLocation(GameRecord->PlayerLocation);
		SPState->LoadState(GameRecord->PlayerHP,GameRecord->PlayerHunger);
		
		//敌人
		int EnemyCount = 0;
		for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt) {
			if (EnemyCount < GameRecord->EnemyLoaction.Num())
			{
				(*EnemyIt)->SetActorLocation(GameRecord->EnemyLoaction[EnemyCount]);
				(*EnemyIt)->LoadHP(GameRecord->EnemyHP[EnemyCount]);
			}
			else {
				(*EnemyIt)->IsDestroyNextTick = true;
			}
			++EnemyCount;
		}

		//资源
		int RockCount = 0;
		for (TActorIterator<ASlAiResourceRock> RockIt(GetWorld()); RockIt; ++RockIt) {
			if (RockCount < GameRecord->ResourceRock.Num()) {
				(*RockIt)->SetActorLocation(GameRecord->ResourceRock[RockCount]);
			}
			else {
				(*RockIt)->IsDestroyNextTick = true;
			}
			++RockCount;
		}
		
		int TreeCount = 0;
		for (TActorIterator<ASlAiResourceTree> TreeIt(GetWorld()); TreeIt; ++TreeIt) {
			if (TreeCount < GameRecord->ResourceTree.Num()) {
				(*TreeIt)->SetActorLocation(GameRecord->ResourceTree[TreeCount]);
			}
			else {
				(*TreeIt)->IsDestroyNextTick = true;
			}
			++TreeCount;
		}
		
		int StoneCount = 0;
		for (TActorIterator<ASlAiPickupStone> StoneIt(GetWorld()); StoneIt; ++StoneIt) {
			if (StoneCount < GameRecord->PickupStone.Num()) {
				(*StoneIt)->SetActorLocation(GameRecord->PickupStone[StoneCount]);
			}
			else {
				(*StoneIt)->IsDestroyNextTick = true;
			}
			++StoneCount;
		}
		
		int WoodCount = 0;
		for (TActorIterator<ASlAiPickupWood> WoodIt(GetWorld()); WoodIt; ++WoodIt) {
			if (WoodCount < GameRecord->PickupWood.Num()) {
				(*WoodIt)->SetActorLocation(GameRecord->PickupWood[WoodCount]);
			}
			else {
				(*WoodIt)->IsDestroyNextTick = true;
			}
			++WoodCount;
		}
	}
}

void ASlAiGameMode::LoadRecordBag()
{
	if (!IsInitBag || !IsNeedLoadRecord)
	{
		return;
	}

	if (IsNeedLoadRecord && GameRecord)
	{
		SlAiBagManager::Get()->LoadRecord(&GameRecord->InputIndex, &GameRecord->InputNum, &GameRecord->NormalIndex, &GameRecord->NormalNum, &GameRecord->ShortcutIndex, &GameRecord->ShortcutNum);
	}

	IsNeedLoadRecord = false;
}

void ASlAiGameMode::SaveGame()
{
	if (SlAiDataHandle::Get()->RecordName.Equals(FString("Default"))) return;

	//创建存档
	USlAiSaveGame* NewRecord = Cast<USlAiSaveGame>(UGameplayStatics::CreateSaveGameObject(USlAiSaveGame::StaticClass()));

	//玩家赋值
	NewRecord->PlayerLocation = SPCharacter->GetActorLocation();
	SPState->SaveState(NewRecord->PlayerHP, NewRecord->PlayerHunger);

	//敌人
	for (TActorIterator<ASlAiEnemyCharacter> EnemyIt(GetWorld()); EnemyIt; ++EnemyIt)
	{
		NewRecord->EnemyLoaction.Add((*EnemyIt)->GetActorLocation());
		NewRecord->EnemyHP.Add((*EnemyIt)->GetHP());
	}

	//各种资源
	for (TActorIterator<ASlAiResourceRock> RockIt(GetWorld()); RockIt; ++RockIt)
	{
		NewRecord->ResourceRock.Add((*RockIt)->GetActorLocation());
	}
	
	for (TActorIterator<ASlAiResourceTree> TreeIt(GetWorld()); TreeIt; ++TreeIt) {
		NewRecord->ResourceTree.Add((*TreeIt)->GetActorLocation());
	}

	for (TActorIterator<ASlAiPickupStone> StoneIt(GetWorld()); StoneIt; ++StoneIt) {
		NewRecord->PickupStone.Add((*StoneIt)->GetActorLocation());
	}
	
	for (TActorIterator<ASlAiPickupWood> WoodIt(GetWorld()); WoodIt; ++WoodIt) {
		NewRecord->PickupWood.Add((*WoodIt)->GetActorLocation());
	}

	SlAiBagManager::Get()->SaveData(NewRecord->InputIndex, NewRecord->InputNum, NewRecord->NormalIndex, NewRecord->NormalNum, NewRecord->ShortcutIndex, NewRecord->ShortcutNum);


	//删除旧的
	if (UGameplayStatics::DoesSaveGameExist(SlAiDataHandle::Get()->RecordName, 0)) {
		UGameplayStatics::DeleteGameInSlot(SlAiDataHandle::Get()->RecordName, 0);
	}
	//保存新的
	UGameplayStatics::SaveGameToSlot(NewRecord, SlAiDataHandle::Get()->RecordName, 0);

	//保存存档记录
	bool IsRecordExist = false;
	for (TArray<FString>::TIterator It(SlAiDataHandle::Get()->RecordDataList); It; ++It)
	{
		if ((*It).Equals(SlAiDataHandle::Get()->RecordName)) {
			IsRecordExist = true;
			break;
		}
	}
	
	if (!IsRecordExist) SlAiDataHandle::Get()->AddNewRecord();

}
