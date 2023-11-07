// Fill out your copyright notice in the Description page of Project Settings.


#include "Bag/SlAiBagWidget.h"

#include "SlAiBagManager.h"
#include "SlAiDataHandle.h"
#include "SlAiGameWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "Bag/Container/SlAIContainerBaseWidget.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiBagWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	GameStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiGameStyle>("BPSlAiGameStyle");
	UIScaler = InArgs._UIScaler;
	IsInitBagManager = false;
	
	MousePosition = FVector2d::Zero();
	
	ChildSlot
	[
		SNew(SOverlay)

		//背包
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.Padding(FMargin(0.f,0.f,50.f,0.f))
		[
			SNew(SBox)
			.WidthOverride(800.f)
			.HeightOverride(800.f)
			[
				SNew(SOverlay)

				//背景图
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(&GameStyle->PackageBGBrush)
				]

				//底部快捷栏
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f,680.f,40.f,40.f))
				[
					SAssignNew(ShortcutGrid,SUniformGridPanel)
				]

				//背包主体
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(40.f,320.f,40.f,160.f))
				[
					SAssignNew(BagGrid,SUniformGridPanel)
				]

				//合成表输入框
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(80.f,40.f,480.f,520.f))
				[
					SAssignNew(CompoundGrid,SUniformGridPanel)
				]

				//合成箭头
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(400.f,120.f,320.f,600.f))
				[
					SNew(SImage)
					.Image(&GameStyle->CompoundArrowBrush)
				]

				//合成表输出框
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(560.f,120.f,160.f,600.f))
				[
					SAssignNew(OutputBorder,SBorder)
				]
			]
		]
	]; 
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiBagWidget::InitBagManger()
{
	//初始化快捷栏
	for (int i = 0; i < 9; ++i)
	{
		//创建实例
		TSharedPtr<SlAIContainerBaseWidget> NewContainer = SlAIContainerBaseWidget::CreateContainer(EContainerType::Shortcut,i);
		//添加
		ShortcutGrid->AddSlot(i,0)[NewContainer->AsShared()];
		SlAiBagManager::Get()->AddContainer(NewContainer,EContainerType::Shortcut);
	}

	//初始化背包
	for (int i = 0; i < 36; ++i)
	{
		//创建实例
		TSharedPtr<SlAIContainerBaseWidget> NewContainer = SlAIContainerBaseWidget::CreateContainer(EContainerType::Normal,i);
		//添加
		BagGrid->AddSlot(i % 9,i / 9)[NewContainer->AsShared()];
		SlAiBagManager::Get()->AddContainer(NewContainer,EContainerType::Normal);
	}

	//初始化合成台
	for (int i = 0; i < 9; ++i)
	{
		//创建实例
		TSharedPtr<SlAIContainerBaseWidget> NewContainer = SlAIContainerBaseWidget::CreateContainer(EContainerType::Input,i);
		//添加
		CompoundGrid->AddSlot(i % 3,i / 3)[NewContainer->AsShared()];
		SlAiBagManager::Get()->AddContainer(NewContainer,EContainerType::Input);
	}

	//输出容器
	TSharedPtr<SlAIContainerBaseWidget> NewContainer = SlAIContainerBaseWidget::CreateContainer(EContainerType::Output,1);
	//添加
	OutputBorder->SetContent(NewContainer->AsShared());
	SlAiBagManager::Get()->AddContainer(NewContainer,EContainerType::Output);

	IsInitBagManager = true;
}

void SlAiBagWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if (GetVisibility() == EVisibility::Visible && GEngine)
	{
		//更新鼠标位置
		GEngine->GameViewport->GetMousePosition(MousePosition);
		MousePosition = MousePosition / UIScaler.Get();

		if (IsInitBagManager)
		{
			SlAiBagManager::Get()->UpdateHoverd(MousePosition,AllottedGeometry);
		}
	}
}

//这只会在这个控件范围内绘制
int32 SlAiBagWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	//先调用父类函数（显示基础的）
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);

	if (!IsInitBagManager)
	{
		return LayerId;
	}

	//在显示并且有物品
	if (GetVisibility() == EVisibility::Visible && SlAiBagManager::Get()->ObjectIndex != 0 &&
		SlAiBagManager::Get()->ObjectNum != 0)
	{
		//渲染物品图标
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 30,		//层级
			AllottedGeometry.ToPaintGeometry(FVector2d(64.f,64.f),
				FSlateLayoutTransform(MousePosition - FVector2d(32.f,32.f))),//绘制位置和大小
			SlAiDataHandle::Get()->ObjectBrushList[SlAiBagManager::Get()->ObjectIndex],//绘制笔刷
			ESlateDrawEffect::None,		//绘制特效
			FLinearColor(1.f,1.f,1.f,1.f)//绘制颜色
		);

		//获取物品属性
		TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(SlAiBagManager::Get()->ObjectIndex);
		//渲染物品数量,不是武器和工具
		if (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon)
		{
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId + 30,		//层级
				AllottedGeometry.ToPaintGeometry(FVector2d(16.f,16.f),
				FSlateLayoutTransform(MousePosition - FVector2d(12.f,16.f))),//绘制位置和大小
				FString::FromInt(SlAiBagManager::Get()->ObjectNum),
				GameStyle->Font_Outline_16,
				ESlateDrawEffect::None,		//绘制特效
				GameStyle->FontColor_Black
			);
		}
	}

	return LayerId;
}

FReply SlAiBagWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!IsInitBagManager)
	{
		return FReply::Handled();
	}

	//左键
	if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		SlAiBagManager::Get()->LeftOption(MousePosition,MyGeometry);
	}

	//右键
	if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		SlAiBagManager::Get()->RightOption(MousePosition,MyGeometry);
	}

	//鼠标位置
	//MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	return FReply::Handled();
}
