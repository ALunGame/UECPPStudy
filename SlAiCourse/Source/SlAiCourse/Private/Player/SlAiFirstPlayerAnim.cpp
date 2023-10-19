// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiFirstPlayerAnim.h"

#include "ConstructorHelpers.h"

USlAiFirstPlayerAnim::USlAiFirstPlayerAnim()
{
	//加载蒙太奇动画
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerHitMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerHitMontage.FirstPlayerHitMontage'"));
	MontageMap.Add(EUpperBodyAnim::Hit,PlayerHitMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerFightMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerFightMontage.FirstPlayerFightMontage'"));
	MontageMap.Add(EUpperBodyAnim::Fight,PlayerFightMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPunchMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerPunchMontage.FirstPlayerPunchMontage'"));
	MontageMap.Add(EUpperBodyAnim::Punch,PlayerPunchMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerEatMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerEatMontage.FirstPlayerEatMontage'"));
	MontageMap.Add(EUpperBodyAnim::Eat,PlayerEatMon.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PlayerPickUpMon(TEXT("/Script/Engine.AnimMontage'/Game/Res/PolygonAdventure/Mannequin/FirstPlayer/Animation/UpperBody/FirstPlayerPickUpMontage.FirstPlayerPickUpMontage'"));
	MontageMap.Add(EUpperBodyAnim::PickUp,PlayerPickUpMon.Object);

	ViewType = EViewType::First;
}
