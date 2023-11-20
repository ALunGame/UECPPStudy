// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiMinMapWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SlAiMinMapWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void RegisterMiniMap(class UTextureRenderTarget2D* MiniMapRender);

	void UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize, const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList,const TArray<float>* EnemyRotateList);

	//virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
private:

	//获取样式
	const struct FSlAiGameStyle *GameStyle;

	//图片
	TSharedPtr<SImage> MiniMapImage;

	TSharedPtr<SImage> EnemyViewImage;

	//敌人视野材质
	UPROPERTY(EditAnywhere,Category="SlAi")
	class UMaterialInstanceDynamic* EnemyViewMatDynamic;

	struct FSlateBrush* MiniMapBrush;
	
	//四个方向的渲染位置
	FVector2D NorthLocation;
	FVector2D SouthLocation;
	FVector2D EastLocation;
	FVector2D WestLocation;

	//小地图尺寸
	float MapSize;

	//敌人相对与玩家的位置
	TArray<FVector2d> EnemyPos;

	//敌人是否锁定玩家
	TArray<bool> EnemyLock;
	
	UPROPERTY(EditAnywhere,Category="SlAi")
	UMaterialInstanceDynamic* MiniMapMatDynamic;
};
