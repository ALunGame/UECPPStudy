// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"

//合成输入委托
DECLARE_DELEGATE(FCompoundInput)
//合成提取委托
DECLARE_DELEGATE_TwoParams(FCompoundOutput,int,int)
//丢弃物品委托
DECLARE_DELEGATE_TwoParams(FThrowObject,int,int)
//背包快捷栏变化
DECLARE_DELEGATE_ThreeParams(FBagShortChange,int,int,int)

/**
 * 
 */
class SLAICOURSE_API SlAIContainerBaseWidget : public SCompoundWidget
{

public:

	FCompoundInput CompoundInput;
	FCompoundOutput CompoundOutput;
	FThrowObject ThrowObject;
	FBagShortChange BagShortChange;
	
public:
	SLATE_BEGIN_ARGS(SlAIContainerBaseWidget)
		{
		}

	SLATE_ATTRIBUTE(int,WorkIndex)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void ResetContainerPara(int ObjectID, int Num);

	//创建容器
	static TSharedPtr<SlAIContainerBaseWidget> CreateContainer(EContainerType::Type NeedType, int WorkID);

	//更新悬浮
	void UpdateHoverd(bool IsHoverd);

	//重置自身属性
	virtual void ResetContainerAttr(int ObjectID,int Num);

	//获得物品Id
	int GetIndex() const;

	//获得物品数量
	int GetNum() const;

	//左键点击操作
	virtual void LeftOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);
	
	//右键点击操作
	virtual void RightOperate(int InputID, int InputNum, int& OutputID, int& OutputNum);

	bool IsEmpty();

	bool RemainSpace(int ObjetID);

	bool AddObject(int ObjetID);
	
protected:
	
	//获取Menu样式
	const struct FSlAiGameStyle *GameStyle;

	//工作序号，在背包的位置
	TAttribute<int> WorkIndex;

	//组件
	TSharedPtr<SBorder> ContainerBorder;
	TSharedPtr<SBorder> ObjectImage;
	TSharedPtr<STextBlock> ObjectNumText;

	bool IsHover;

	//物品序号
	int ObjectIndex;
	//数量
	int ObjectNum;

protected:

	//是否可叠加
	bool MultiplyAble(int ObjectID);
};




