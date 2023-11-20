// Fill out your copyright notice in the Description page of Project Settings.


#include "MinMap/SlAiMinMapWidget.h"

#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiMinMapWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(320.f)
		.HeightOverride(320.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&GameStyle->MinmapBGBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(MiniMapImage,SImage)
			]

			//渲染敌人图片
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SAssignNew(EnemyViewImage,SImage)
			]
		]
	]; 
}

void SlAiMinMapWidget::RegisterMiniMap(UTextureRenderTarget2D* MiniMapRender)
{
	//获得材质
	UMaterialInterface* MiniMapMatInst = LoadObject<UMaterialInterface>(nullptr,TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/MinMapMat_Inst.MinMapMat_Inst'"));	
	//创建材质
	MiniMapMatDynamic = UMaterialInstanceDynamic::Create(MiniMapMatInst,nullptr);
	//设置属性
	MiniMapMatDynamic->SetTextureParameterValue(FName("MinMapTex"),MiniMapRender);
	//创建笔刷
	MiniMapBrush = new FSlateBrush();
	//设置属性
	MiniMapBrush->ImageSize = FVector2d(280.f,280.f);
	MiniMapBrush->DrawAs = ESlateBrushDrawType::Image;
	//绑定材质文件
	MiniMapBrush->SetResourceObject(MiniMapMatDynamic);
	//设置笔刷
	MiniMapImage->SetImage(MiniMapBrush);

	//敌人视野材质
	UMaterialInterface* EnemyViewInst = LoadObject<UMaterialInterface>(nullptr,TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/EnemyViewMat_Inst.EnemyViewMat_Inst'"));	
	EnemyViewMatDynamic = UMaterialInstanceDynamic::Create(EnemyViewInst,nullptr);
	//笔刷
	FSlateBrush* EnemyViewBrush = new FSlateBrush();
	//设置属性
	EnemyViewBrush->ImageSize = FVector2d(280.f,280.f);
	EnemyViewBrush->DrawAs = ESlateBrushDrawType::Image;
	//绑定材质文件
	EnemyViewBrush->SetResourceObject(EnemyViewMatDynamic);
	//设置笔刷
	EnemyViewImage->SetImage(EnemyViewBrush);
	//设置颜色
	EnemyViewImage->SetColorAndOpacity(FLinearColor(0.3f,0.f,0.32f,0.4f));
}

void SlAiMinMapWidget::UpdateMapData(const FRotator PlayerRotator, const float MiniMapSize,
	const TArray<FVector2D>* EnemyPosList, const TArray<bool>* EnemyLockList, const TArray<float>* EnemyRotateList)
{
	//获取方向
	float YawDir = -PlayerRotator.Yaw;

	//计算方向位置
	NorthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir)), FMath::Cos(FMath::DegreesToRadians(YawDir))) * 150.f + FVector2D(160.f, 160.f);
	EastLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 90.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 90.f)))*150.f + FVector2D(160.f, 160.f);
	SouthLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 180.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 180.f)))*150.f + FVector2D(160.f, 160.f);
	WestLocation = FVector2D(FMath::Sin(FMath::DegreesToRadians(YawDir + 270.f)), FMath::Cos(FMath::DegreesToRadians(YawDir + 270.f)))*150.f + FVector2D(160.f, 160.f);

	//地图尺寸
	MapSize = MiniMapSize;
	EnemyPos.Empty();
	EnemyLock.Empty();

	//比例
	float DPIRatio = 280.f / MapSize;

	//旋转
	TArray<float> EnemyViewRotate;
	//位置
	TArray<FVector2D> EnemyViewPos;
	//锁定
	TArray<bool> EnemyViewLock;

	//敌人信息
	for (int i = 0; i < EnemyPosList->Num(); ++i) {
		//实际长度
		float RealDistance = (*EnemyPosList)[i].Size();
		//长度小于地图半径
		if (RealDistance * 2 < MapSize)
		{
			//屏幕位置
			EnemyPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			//锁定玩家
			EnemyLock.Add((*EnemyLockList)[i]);
		}
		//如果长度小于地图实际半径加2000，就渲染
		if (RealDistance * 2 < MapSize + 2000.f)
		{
			//屏幕位置
			EnemyViewPos.Add((*EnemyPosList)[i] * DPIRatio + FVector2D(160.f, 160.f));
			//锁定玩家
			EnemyViewLock.Add((*EnemyLockList)[i]);
			//添加旋转，并格式化为0-1
			float RotVal = -(*EnemyRotateList)[i];
			if (RotVal > 180.f) RotVal -= 360.f;
			if (RotVal < -180.f) RotVal += 360.f;
			//0-360
			RotVal += 180.f;
			//0-1
			RotVal /= 360.f;
			//转180
			RotVal = RotVal + 0.5f > 1.f ? RotVal - 0.5f : RotVal + 0.5f;
			EnemyViewRotate.Add(RotVal);
		}
	}

	int ViewCount = 0;
	//设置尺寸
	EnemyViewMatDynamic->SetScalarParameterValue(FName("Scale"), 1000.f / MapSize);

	if (!EnemyViewPos.IsEmpty() && EnemyViewPos.Num() > 0)
	{
		for (int i = 0; i < EnemyViewPos.Num(); ++i,++ViewCount) {
			if (i + 1 <= 10)
			{
				FString PosName = FString("Position_") + FString::FromInt(i + 1);
				FString AngleName = FString("Angle_") + FString::FromInt(i + 1);

				//锁定玩家
				if (!EnemyViewLock[i]) {
					EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor((EnemyViewPos[i].X - 20.f) / 280.f, (EnemyViewPos[i].Y - 20.f) / 280.f, 0.f, 0.f));
					EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), EnemyViewRotate[i]);
				}
				else
				{
					EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
					EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
				}
			}
		}
	}

	//剩下的都不渲染
	for (ViewCount += 1; ViewCount < 11; ++ViewCount) {
		if (ViewCount <= 10)
		{
			FString PosName = FString("Position_") + FString::FromInt(ViewCount);
			FString AngleName = FString("Angle_") + FString::FromInt(ViewCount);
			EnemyViewMatDynamic->SetVectorParameterValue(FName(*PosName), FLinearColor(0.f, 0.f, 0.f, 0.f));
			EnemyViewMatDynamic->SetScalarParameterValue(FName(*AngleName), 0.f);
		}
	}
}

// int32 SlAiMinMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
// 	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
// 	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
// {
// 	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
// 	                                bParentEnabled);
//
// 	return LayerId;
//
// 	//渲染玩家坐标
// 	FSlateDrawElement::MakeBox(
// 		OutDrawElements,
// 		LayerId + 100,		//层级
// 		AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
// 			FSlateLayoutTransform(FVector2d(150.f,150.f))),//绘制位置和大小
// 		&GameStyle->PawnPointBrush,//绘制笔刷
// 		ESlateDrawEffect::None,		//绘制特效
// 		FLinearColor(1.f,1.f,0.f,1.f)//绘制颜色
// 	);
// 	
//
// 	//绘制方向文字
// 	FSlateDrawElement::MakeText(
// 		OutDrawElements,
// 		LayerId + 10,		//层级
// 		AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
// 			FSlateLayoutTransform(NorthLocation - FVector2d(8.f,8.f))),//绘制位置和大小
// 		NSLOCTEXT("SlAiGame","N","N"),
// 		GameStyle->Font_16,
// 		ESlateDrawEffect::None,		//绘制特效
// 		FLinearColor(1.f,1.f,1.f,1.f)//绘制颜色
// 	);
// 	FSlateDrawElement::MakeText(
// 		OutDrawElements,
// 		LayerId + 10,		//层级
// 		AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
// 			FSlateLayoutTransform(SouthLocation - FVector2d(8.f,8.f))),//绘制位置和大小
// 		NSLOCTEXT("SlAiGame","S","S"),
// 		GameStyle->Font_16,
// 		ESlateDrawEffect::None,		//绘制特效
// 		FLinearColor(1.f,1.f,1.f,1.f)//绘制颜色
// 	);
// 	FSlateDrawElement::MakeText(
// 	OutDrawElements,
// 		LayerId + 10,		//层级
// 		AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
// 		FSlateLayoutTransform(EastLocation - FVector2d(8.f,8.f))),//绘制位置和大小
// 		NSLOCTEXT("SlAiGame","E","E"),
// 		GameStyle->Font_16,
// 		ESlateDrawEffect::None,		//绘制特效
// 		FLinearColor(1.f,1.f,1.f,1.f)//绘制颜色
// 	);
// 	FSlateDrawElement::MakeText(
// 		OutDrawElements,
// 		LayerId + 10,		//层级
// 		AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
// 		FSlateLayoutTransform(WestLocation - FVector2d(8.f,8.f))),//绘制位置和大小
// 		NSLOCTEXT("SlAiGame","W","W"),
// 		GameStyle->Font_16,
// 		ESlateDrawEffect::None,		//绘制特效
// 		FLinearColor(1.f,1.f,1.f,1.f)//绘制颜色
// 	);
//
// 	//渲染敌人位置
// 	for (int i = 0; i < EnemyPos.Num(); ++i) {
// 		//渲染图标
// 		FSlateDrawElement::MakeBox(
// 			OutDrawElements,
// 			LayerId + 100,
// 			AllottedGeometry.ToPaintGeometry(FVector2D(10.f, 10.f),FSlateLayoutTransform(EnemyPos[i] - FVector2D(5.f, 5.f))),
// 			&GameStyle->PawnPointBrush,
// 			ESlateDrawEffect::None,
// 			EnemyLock[i] ? FLinearColor(1.f, 0.f, 0.f, 1.f) : FLinearColor(0.f, 1.f, 0.f, 1.f)
// 		);
// 	}
//
// 	return LayerId;
// }

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
