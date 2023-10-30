﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiPlayerState.h"

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "SlAiTypes.h"

ASlAiPlayerState::ASlAiPlayerState()
{
	//当前选中的快捷栏Id
	CurrShortcutIndex = 0;
}

void ASlAiPlayerState::OnRegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* Containers,
	TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	SlAiHelper::DebugError("ASlAiPlayerState::OnRegisterShortcutContainer",600.f);
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*Containers); It; ++It) {
		ShortcutContainers.Add(*It);
	}
	//绑定属性
	ShortcutInfoTextAttr.BindUObject(this,&ASlAiPlayerState::GetShortcutInfoText);
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);

	//测试
	SlAiHelper::DebugError("ShortcutContainers.Num: "+ShortcutContainers.Num(),600.f);
	ShortcutContainers[1]->SetObject(1)->SetObjectNum(5);
	ShortcutContainers[2]->SetObject(2)->SetObjectNum(15);
	ShortcutContainers[3]->SetObject(3)->SetObjectNum(2);
	ShortcutContainers[4]->SetObject(4)->SetObjectNum(4);
	ShortcutContainers[5]->SetObject(5)->SetObjectNum(5);
	ShortcutContainers[6]->SetObject(6)->SetObjectNum(7);
	ShortcutContainers[7]->SetObject(7)->SetObjectNum(55);
}

void ASlAiPlayerState::ChooseShortcut(bool IsUp)
{
	//下一个选择的下标
	int NextIndex = 0;
	int TotalIndex = ShortcutContainers.Num();
	
	if (IsUp)
	{
		NextIndex = CurrShortcutIndex - 1 < 0 ? TotalIndex - 1 : CurrShortcutIndex - 1;
	}
	else
	{
		NextIndex = CurrShortcutIndex + 1 >= TotalIndex ? 0 : CurrShortcutIndex + 1;
	}
	
	ShortcutContainers[CurrShortcutIndex]->SetChoose(false);
	ShortcutContainers[NextIndex]->SetChoose(true);
	
	CurrShortcutIndex = NextIndex;
}

int ASlAiPlayerState::GetCurrHandleObjectIndex() const
{
	if (ShortcutContainers.Num() == 0)
	{
		return 0;
	}

	return ShortcutContainers[CurrShortcutIndex]->ObjectIndex;
}

TSharedPtr<ObjectAttr> ASlAiPlayerState::GetCurrHandleObjectAttr() const
{
	if (ShortcutContainers.Num() == 0)
	{
		return nullptr;
	}
	TSharedPtr<ObjectAttr> ObjectAttr = *SlAiDataHandle::Get()->ObjectAttrMap.Find(GetCurrHandleObjectIndex());
	return ObjectAttr;
}

void ASlAiPlayerState::RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock)
{
	//绑定属性
	RayInfoTextAttr.BindUObject(this,&ASlAiPlayerState::GetRayInfoText);
	RayInfoTextBlock->SetText(ShortcutInfoTextAttr);
}

FText ASlAiPlayerState::GetShortcutInfoText() const
{
	TSharedPtr<ObjectAttr> ObjectAttr = GetCurrHandleObjectAttr();
	switch (SlAiDataHandle::Get()->CurCulture)
	{
	case ECultureTeam::EN:
		return ObjectAttr->EN;
	case ECultureTeam::ZH:
		return ObjectAttr->ZH;
	}
	return ObjectAttr->ZH;

	//return FText::FromString(FString("aaa"));
}

FText ASlAiPlayerState::GetRayInfoText() const
{
	SlAiHelper::Debug("GetRayInfoText>>>>",100.f);
	return FText::FromString("hhhh");
}
