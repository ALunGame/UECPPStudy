// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiPlayerAnim.h"

#include "SlAiPlayerController.h"

USlAiPlayerAnim::USlAiPlayerAnim()
{
	Speed = 0.f;
	SpineRotator = FRotator(0.f,0.f,0.f);
}

void USlAiPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	InitSPCharacter();
	if (SPCharacter)
	{
		UpdateParam();
		UpdateMontage();
	}
}

void USlAiPlayerAnim::ChangeDetection(bool IsOpen)
{
	if (!SPCharacter)
	{
		return;
	}
	SPCharacter->ChangeHandObjectDetect(IsOpen);
}

void USlAiPlayerAnim::RenderHandleObject(bool IsOpen)
{
	if (!SPCharacter)
    {
    	return;
    }
	SPCharacter->ChangeHandObjectRender(IsOpen);
}

void USlAiPlayerAnim::UpdateMontage()
{
	if (SPCharacter->MineController == nullptr)
	{
		return;
	}
	
	//当前视角不等于自身的视角
	if (SPCharacter->MineController->CurrViewType!=ViewType)
	{
		return;
	}
	
	//当前动作没有停止返回
	if (CurrentMontage && !Montage_GetIsStopped(CurrentMontage))
	{
		return;
	}
	
	EUpperBodyAnim::Type CurrUpperBodyAnim = SPCharacter->UpperBodyAnim;
	
	if (!MontageMap.Contains(CurrUpperBodyAnim))
	{
		Montage_Stop(0);
		CurrentMontage = nullptr;
		AllowChangeView(true);
		return;
	}
	
	if (!Montage_IsPlaying(CurrentMontage))
	{
		CurrentMontage = MontageMap[CurrUpperBodyAnim];
		Montage_Play(CurrentMontage);
		AllowChangeView(false);
	}
}

void USlAiPlayerAnim::AllowChangeView(bool IsAllow)
{
	if (!SPCharacter)
	{
		return;
	}

	SPCharacter->IsAllowSwitchView = IsAllow;
}

void USlAiPlayerAnim::InitSPCharacter()
{
	if (!SPCharacter)
	{
		SPCharacter = Cast<ASlAiPlayerCharacter>(TryGetPawnOwner());
	}
}

void USlAiPlayerAnim::UpdateParam()
{
	//速度
	Speed = SPCharacter->GetVelocity().Size();

	//旋转
	float SpineDir = SPCharacter->GetActorRotation().Yaw - 90.f;
	if (SpineDir > 180.f)
	{
		SpineDir -= 360.f;
	}
	if (SpineDir < -180.f)
	{
		SpineDir += 360.f;
	}
	SpineRotator = FRotator(0.f,SpineDir,90.f);
}
