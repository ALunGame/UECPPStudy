// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiSceneCapture2D.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"


// Sets default values
ASlAiSceneCapture2D::ASlAiSceneCapture2D()
{
	//设置每帧更新
	GetCaptureComponent2D()->bCaptureEveryFrame = true;

	//设置渲染格式
	GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;

	//正交模式
	GetCaptureComponent2D()->ProjectionType = ECameraProjectionMode::Orthographic;

	///范围
	GetCaptureComponent2D()->OrthoWidth = 3000.f;

	//设置旋转头嘲下
	SetActorRotation(FRotator(-90.f,0.f,0.f));
}

UTextureRenderTarget2D* ASlAiSceneCapture2D::GetMiniMapTex()
{
	MiniMapTex = NewObject<UTextureRenderTarget2D>();
	MiniMapTex->InitAutoFormat(256,256);

	GetCaptureComponent2D()->TextureTarget = MiniMapTex;

	return MiniMapTex;
}

void ASlAiSceneCapture2D::UpdateTransform(FVector NormalLocation, FRotator NormalRotator)
{
	//玩家位置上方1000
	SetActorLocation(NormalLocation + FVector(0.f,0.f,1000.f));
	//旋转
	SetActorRotation(FRotator(-90.f,NormalRotator.Yaw,NormalRotator.Roll));
}

void ASlAiSceneCapture2D::UpdateMiniMapWidth(int Delta)
{
	const float PreWidth = GetCaptureComponent2D()->OrthoWidth;
	GetCaptureComponent2D()->OrthoWidth = FMath::Clamp<float>(PreWidth + Delta, 2000.f, 4000.f);
}

float ASlAiSceneCapture2D::GetMapSize()
{
	return GetCaptureComponent2D()->OrthoWidth;
}


