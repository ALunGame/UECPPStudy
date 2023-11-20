// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Container/SlAIContainerBaseWidget.h"

/**
 * 
 */
class SLAICOURSE_API SlAiBagManager
{

public:

	//鼠标物品Id
	int ObjectIndex;
	//鼠标物品数量
	int ObjectNum;

	//丢弃物品
	FThrowObject PlayerThrowObject;
	FBagShortChange ChangeHandObject;
	
public:
	SlAiBagManager();

	static void Init();

	static TSharedPtr<SlAiBagManager> Get();

	//添加容器
	void AddContainer(TSharedPtr<SlAIContainerBaseWidget> Container, EContainerType::Type AddType);

	//更新悬停容器颜色
	void UpdateHoverd(FVector2d MousePos,FGeometry PackGeo);

	//左键
	void LeftOption(FVector2d MousePos,FGeometry PackGeo);
	//右键
	void RightOption(FVector2d MousePos,FGeometry PackGeo);

	
	bool SearchFreeSpace(int ObjectID,TSharedPtr<SlAIContainerBaseWidget>& FreeContainer);
	
	bool SearchFreeSpace(int ObjectID);

	bool AddObject(int ObjectID);

	bool EatUpEvent(int ShortcutID);
	
	void LoadRecord(TArray<int32>* InputIndex, TArray<int32>* InputNum, TArray<int32>* NormalIndex, TArray<int32>* NormalNum, TArray<int32>* ShortcutIndex, TArray<int32>* ShortcutNum);
	void SaveData(TArray<int32>& InputIndex, TArray<int32>& InputNum, TArray<int32>& NormalIndex, TArray<int32>& NormalNum, TArray<int32>& ShortcutIndex, TArray<int32>& ShortcutNum);

private:
	static TSharedRef<SlAiBagManager> Create();
	
	static TSharedPtr<SlAiBagManager> Instance;

	//容器列表
	TArray<TSharedPtr<SlAIContainerBaseWidget>> InputContainerList;
	TArray<TSharedPtr<SlAIContainerBaseWidget>> NormalContainerList;
	TArray<TSharedPtr<SlAIContainerBaseWidget>> ShortcutContainerList;
	//输出容器
	TSharedPtr<SlAIContainerBaseWidget> OutputContainer;

	//上一个悬浮的肉哦那个其
	TSharedPtr<SlAIContainerBaseWidget> LastHoverdContainer;

private:

	//获取鼠标指向的容器
	TSharedPtr<SlAIContainerBaseWidget> LocateContainer(FVector2d MousePos,FGeometry PackGeo);

	//丢弃物品事件
	void ThrowObject(int ObjectID, int Num);
	//合成提取事件
	void CompoundOutput(int ObjectID, int Num);
	//合成输入事件
	void CompoundInput();
	//合成输入事件
	void BagShortChange(int ShortcutID, int ObjectID, int ObjectNum);
	
	//是否可以堆叠
	bool MultiplyAble(int ObjectID);

};
