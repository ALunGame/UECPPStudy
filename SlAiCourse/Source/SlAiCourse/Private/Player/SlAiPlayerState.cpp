// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiPlayerState.h"

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "SlAiTypes.h"

ASlAiPlayerState::ASlAiPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//当前选中的快捷栏Id
	CurrShortcutIndex = 0;

	HP = 500;
	Hunger = 600;
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

void ASlAiPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Hunger <= 0)
	{
		HP -= DeltaSeconds * 2;
	}
	else
	{
		Hunger -= DeltaSeconds * 2;
		HP += DeltaSeconds;
	}

	//设定范围
	HP = FMath::Clamp(HP,0.f,500.f);
	Hunger = FMath::Clamp(Hunger,0.f,600.f);

	//执行委托
	UpdateStateWidget.ExecuteIfBound(HP/500.f,Hunger/600.f);
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
	RayInfoTextBlock->SetText(RayInfoTextAttr);
}

int ASlAiPlayerState::GetAffectRange()
{
	TSharedPtr<ObjectAttr> ObjectAttr = GetCurrHandleObjectAttr();
	return ObjectAttr->AffectRange;
}

int ASlAiPlayerState::GetDamageValue(EResourceType::Type ResourceType)
{
	TSharedPtr<ObjectAttr> ObjectAttr = GetCurrHandleObjectAttr();
	switch (ResourceType)
	{
	case EResourceType::Plant:
		return ObjectAttr->PlantAttack;
	case EResourceType::Metal:
		return ObjectAttr->MetalAttcck;
	case EResourceType::Animal:
		return ObjectAttr->AnimalAttack;
	}
	return ObjectAttr->PlantAttack;
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
	return RayInfoText;
}
