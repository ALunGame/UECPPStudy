// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiPointerWidget.h"

#include "ConstructorHelpers.h"
#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Materials/MaterialInstanceDynamic.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiPointerWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");

	//初始化
	CurrSize = 20.f;
	IsAnimed = false;
	
	ChildSlot
	[
		SAssignNew(RootBox,SBox)
		.WidthOverride(TAttribute<FOptionalSize>(this, &SlAiPointerWidget::GetBoxSize))
		.HeightOverride(TAttribute<FOptionalSize>(this, &SlAiPointerWidget::GetBoxSize))
		[
			SNew(SImage)
			.Image(&GameStyle->PointerBrush)
		]
	];

	//加载材质
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> StaticPointerMatInstance(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/PointerMat_Inst.PointerMat_Inst'"));
	//转换为动态材质
	PointerMaterial = (UMaterialInstanceDynamic*)StaticPointerMatInstance.Object;
}

void SlAiPointerWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	//实施修改
	//插值改变
	CurrSize = IsAnimed ? FMath::FInterpTo(CurrSize,130.f,InDeltaTime,10.f) : FMath::FInterpTo(CurrSize,20.f,InDeltaTime,10.f);
}

void SlAiPointerWidget::UpdatePointer(bool IsAnim, float Range)
{
	IsAnimed = IsAnim;
	PointerMaterial->SetScalarParameterValue(FName("Range"),Range);
}

FOptionalSize SlAiPointerWidget::GetBoxSize() const
{
	return FOptionalSize(CurrSize);
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
