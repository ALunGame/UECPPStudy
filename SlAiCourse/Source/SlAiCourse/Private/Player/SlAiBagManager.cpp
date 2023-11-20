// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiBagManager.h"

#include "SlAiDataHandle.h"

TSharedPtr<SlAiBagManager> SlAiBagManager::Instance = nullptr;

void SlAiBagManager::Init()
{
	if (!Instance.IsValid())
	{
		Instance = Create();
	}
}

TSharedPtr<SlAiBagManager> SlAiBagManager::Get()
{
	Init();
	return Instance;
}

void SlAiBagManager::AddContainer(TSharedPtr<SlAIContainerBaseWidget> Container, EContainerType::Type AddType)
{
	switch (AddType)
	{
	case EContainerType::Output:
		OutputContainer = Container;
		OutputContainer->CompoundOutput.BindRaw(this, &SlAiBagManager::CompoundOutput);
		OutputContainer->ThrowObject.BindRaw(this, &SlAiBagManager::ThrowObject);
		break;
	case EContainerType::Input:
		InputContainerList.Add(Container);
		Container->CompoundInput.BindRaw(this,&SlAiBagManager::CompoundInput);
		break;
	case EContainerType::Normal:
		NormalContainerList.Add(Container);
		break;
	case EContainerType::Shortcut:
		ShortcutContainerList.Add(Container);
		Container->BagShortChange.BindRaw(this,&SlAiBagManager::BagShortChange);
		break;
	default: ;
	}
}

void SlAiBagManager::UpdateHoverd(FVector2d MousePos, FGeometry PackGeo)
{
	TSharedPtr<SlAIContainerBaseWidget> CurrHoverCon = LocateContainer(MousePos,PackGeo);
	if (CurrHoverCon.IsValid())
	{
		CurrHoverCon->UpdateHoverd(true);

		if (LastHoverdContainer.IsValid() && LastHoverdContainer.Get() != CurrHoverCon.Get())
		{
			LastHoverdContainer->UpdateHoverd(false);
		}
	}
	else
	{
		if (LastHoverdContainer.IsValid())
		{
			LastHoverdContainer->UpdateHoverd(false);
		}
	}

	LastHoverdContainer = CurrHoverCon;
}

void SlAiBagManager::LeftOption(FVector2d MousePos, FGeometry PackGeo)
{
	TSharedPtr<SlAIContainerBaseWidget> ClickCon = LocateContainer(MousePos,PackGeo);
	if (ClickCon.IsValid())
	{
		ClickCon->LeftOperate(ObjectIndex,ObjectNum,ObjectIndex,ObjectNum);
	}

	if (!ClickCon.IsValid() && ObjectIndex != 0)
	{
		ThrowObject(ObjectIndex,ObjectNum);
		ObjectIndex = ObjectNum = 0;
	}
}

void SlAiBagManager::RightOption(FVector2d MousePos, FGeometry PackGeo)
{
	TSharedPtr<SlAIContainerBaseWidget> ClickCon = LocateContainer(MousePos,PackGeo);
	if (ClickCon.IsValid())
	{
		ClickCon->RightOperate(ObjectIndex,ObjectNum,ObjectIndex,ObjectNum);
	}
}

TSharedRef<SlAiBagManager> SlAiBagManager::Create()
{
	TSharedRef<SlAiBagManager> dataRef = MakeShareable(new SlAiBagManager());
	return dataRef;
}

TSharedPtr<SlAIContainerBaseWidget> SlAiBagManager::LocateContainer(FVector2d MousePos, FGeometry PackGeo)
{
	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It)
	{
		//获取区域
		FVector2d StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2d EndPos = StartPos + FVector2d(80.f,80.f);

		//判断区域
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		{
			return (*It);
		}
	}

	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It)
	{
		//获取区域
		FVector2d StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2d EndPos = StartPos + FVector2d(80.f,80.f);

		//判断区域
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		{
			return (*It);
		}
	}

	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It)
	{
		//获取区域
		FVector2d StartPos = PackGeo.AbsoluteToLocal((*It)->GetCachedGeometry().AbsolutePosition);
		FVector2d EndPos = StartPos + FVector2d(80.f,80.f);

		//判断区域
		if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
		{
			return (*It);
		}
	}

	//输出容器
	//获取区域
	FVector2d StartPos = PackGeo.AbsoluteToLocal(OutputContainer->GetCachedGeometry().AbsolutePosition);
	FVector2d EndPos = StartPos + FVector2d(80.f,80.f);
	//判断区域
	if (MousePos.X >= StartPos.X && MousePos.X <= EndPos.X && MousePos.Y >= StartPos.Y && MousePos.Y <= EndPos.Y)
	{
		return OutputContainer;
	}

	return nullptr;
}

void SlAiBagManager::ThrowObject(int ObjectID, int Num)
{
	PlayerThrowObject.ExecuteIfBound(ObjectID,Num);
}

void SlAiBagManager::BagShortChange(int ShortcutID, int ObjectID, int ObjectNum)
{
	ChangeHandObject.ExecuteIfBound(ShortcutID,ObjectID,ObjectNum);
}

void SlAiBagManager::CompoundOutput(int ObjectID, int Num)
{
	if (ObjectID == 0) return;
	//当前Id
	TArray<int> TableMap;
	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		TableMap.Add((*It)->GetIndex());
	}
	TableMap.Add(ObjectID);
	
	//获得消耗
	TArray<int> ExpendMap;
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It) {
		if ((*It)->DetectExpend(&TableMap, Num, ExpendMap)) break;
	}
	//9号位是输出
	if (ExpendMap.Num() != 9) return;
	
	//更新
	for (int i = 0; i < 9; ++i) {
		int InputID = (InputContainerList[i]->GetNum() - ExpendMap[i] <= 0) ? 0 : InputContainerList[i]->GetIndex();
		int InputNum = (InputID == 0) ? 0 : (InputContainerList[i]->GetNum() - ExpendMap[i]);
		InputContainerList[i]->ResetContainerAttr(InputID, InputNum);
	}
}

void SlAiBagManager::CompoundInput()
{
	//当前Id和数量
	TArray<int> IDMap;
	TArray<int> NumMap;
	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(InputContainerList); It; ++It) {
		IDMap.Add((*It)->GetIndex());
		NumMap.Add((*It)->GetNum());
	}

	//检测出来的合成道具
	int OutputIndex = 0;
	int OutputNum = 0;
	for (TArray<TSharedPtr<CompoundTable>>::TIterator It(SlAiDataHandle::Get()->CompoundTableMap); It; ++It) {
		(*It)->DetectTable(&IDMap, &NumMap, OutputIndex, OutputNum);
		if (OutputIndex != 0 && OutputNum != 0) break;
	}

	if (MultiplyAble(OutputIndex)) {
		OutputContainer->ResetContainerAttr(OutputIndex, OutputNum);
	}
	else {
		OutputContainer->ResetContainerAttr(OutputIndex, 1);
	}
}

bool SlAiBagManager::MultiplyAble(int ObjectID)
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ObjectID);
	return (ObjectAttr->ObjectType != EObjectType::Tool && ObjectAttr->ObjectType != EObjectType::Weapon);
}

bool SlAiBagManager::SearchFreeSpace(int ObjectID, TSharedPtr<SlAIContainerBaseWidget>& FreeContainer)
{
	TSharedPtr<SlAIContainerBaseWidget> EmptyContainer;

	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(ShortcutContainerList); It; ++It)
	{
		if (!EmptyContainer.IsValid())
		{
			if ((*It)->IsEmpty())
			{
				EmptyContainer = *It;
			}
		}
		if (!FreeContainer.IsValid())
		{
			if ((*It)->RemainSpace(ObjectID))
			{
				FreeContainer = *It;
				return true;
			}
		}
	}

	for (TArray<TSharedPtr<SlAIContainerBaseWidget>>::TIterator It(NormalContainerList); It; ++It)
	{
		if (!EmptyContainer.IsValid())
		{
			if ((*It)->IsEmpty())
			{
				EmptyContainer = *It;
			}
		}
		if (!FreeContainer.IsValid())
		{
			if ((*It)->RemainSpace(ObjectID))
			{
				FreeContainer = *It;
				return true;
			}
		}
	}

	if (EmptyContainer.IsValid())
	{
		FreeContainer = EmptyContainer;
		return true;
	}

	return false;
}

bool SlAiBagManager::SearchFreeSpace(int ObjectID)
{
	TSharedPtr<SlAIContainerBaseWidget> FreeContainer;
	return SearchFreeSpace(ObjectID,FreeContainer);
}

bool SlAiBagManager::AddObject(int ObjectID)
{
	TSharedPtr<SlAIContainerBaseWidget> FreeContainer;
	if (SearchFreeSpace(ObjectID,FreeContainer))
	{
		return FreeContainer->AddObject(ObjectID);
	}
	return false;
}

bool SlAiBagManager::EatUpEvent(int ShortcutID)
{
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(ShortcutContainerList[ShortcutID]->GetIndex());
	if (ObjectAttr->ObjectType == EObjectType::Food)
	{
		int NewNum = ShortcutContainerList[ShortcutID]->GetNum() - 1 < 0 ? 0 : ShortcutContainerList[ShortcutID]->GetNum() - 1;
		int NewIndex = NewNum == 0 ? 0 : ShortcutContainerList[ShortcutID]->GetIndex();
		ShortcutContainerList[ShortcutID]->ResetContainerAttr(NewIndex,NewNum);
		return true;
	}
	return false;
}

void SlAiBagManager::LoadRecord(TArray<int32>* InputIndex, TArray<int32>* InputNum, TArray<int32>* NormalIndex,
	TArray<int32>* NormalNum, TArray<int32>* ShortcutIndex, TArray<int32>* ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		if ((*InputIndex)[i] != 0) InputContainerList[i]->ResetContainerPara((*InputIndex)[i], (*InputNum)[i]);
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i) {
		if ((*NormalIndex)[i] != 0) NormalContainerList[i]->ResetContainerPara((*NormalIndex)[i], (*NormalNum)[i]);
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i) {
		if ((*ShortcutIndex)[i] != 0) ShortcutContainerList[i]->ResetContainerPara((*ShortcutIndex)[i], (*ShortcutNum)[i]);
	}
}

void SlAiBagManager::SaveData(TArray<int32>& InputIndex, TArray<int32>& InputNum, TArray<int32>& NormalIndex,
	TArray<int32>& NormalNum, TArray<int32>& ShortcutIndex, TArray<int32>& ShortcutNum)
{
	for (int i = 0; i < InputContainerList.Num(); ++i)
	{
		InputIndex.Add(InputContainerList[i]->GetIndex());
		InputNum.Add(InputContainerList[i]->GetNum());
	}
	for (int i = 0; i < NormalContainerList.Num(); ++i) {
		NormalIndex.Add(NormalContainerList[i]->GetIndex());
		NormalNum.Add(NormalContainerList[i]->GetNum());
	}
	for (int i = 0; i < ShortcutContainerList.Num(); ++i) {
		ShortcutIndex.Add(ShortcutContainerList[i]->GetIndex());
		ShortcutNum.Add(ShortcutContainerList[i]->GetNum());
	}
}

SlAiBagManager::SlAiBagManager()
{
	ObjectIndex = 0;
	ObjectNum = 0;
}

