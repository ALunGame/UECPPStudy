// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SlAiPlayerState.h"

#include "SlAiDataHandle.h"
#include "SlAiHelper.h"
#include "SlAiPlayerController.h"
#include "SlAiTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SlAiPlayerController.h"

ASlAiPlayerState::ASlAiPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	
	//当前选中的快捷栏Id
	CurrShortcutIndex = 0;

	HP = 500;
	Hunger = 600;

	IsDead = false;
}

void ASlAiPlayerState::OnRegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* Containers,
	TSharedPtr<STextBlock> ShortcutInfoTextBlock)
{
	for (TArray<TSharedPtr<ShortcutContainer>>::TIterator It(*Containers); It; ++It) {
		ShortcutContainers.Add(*It);
	}
	//绑定属性
	ShortcutInfoTextAttr.BindUObject(this,&ASlAiPlayerState::GetShortcutInfoText);
	ShortcutInfoTextBlock->SetText(ShortcutInfoTextAttr);
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
		if (!IsDead)
		{
			Hunger -= DeltaSeconds * 2;
			HP += DeltaSeconds;
		}
	}

	//设定范围
	HP = FMath::Clamp(HP,0.f,500.f);
	Hunger = FMath::Clamp(Hunger,0.f,600.f);

	//执行委托
	UpdateStateWidget.ExecuteIfBound(HP/500.f,Hunger/600.f);

	if (HP == 0.f && !IsDead)
	{
		IsDead = true;
		if (SPController)
		{
			SPController->PlayerDead();
		}
	}
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

void ASlAiPlayerState::ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum)
{
	ShortcutContainers[ShortcutID]->SetObject(ObjectID)->SetObjectNum(ObjectNum);
	SPController->ChangeHandObject();
}

void ASlAiPlayerState::PromoteHungry()
{
	if (Hunger + 100 >= 500.f)
	{
		Hunger = 600.f;
		return;
	}
	Hunger = FMath::Clamp<float>(Hunger + 100.f,0,600.f);
}

bool ASlAiPlayerState::IsPlayerDead()
{
	return HP <= 0.f;
}

void ASlAiPlayerState::AcceptDamage(int DamageVal)
{
	HP = FMath::Clamp<float>(HP - DamageVal, 0.f, 500.f);
	UpdateStateWidget.ExecuteIfBound(HP / 500.f, Hunger / 500.f);
	if (HP == 0.F && !IsDead)
	{
		IsDead = true;
		if (SPController)
		{
			SPController->PlayerDead();
		}
	}
	else
	{
		
	}
}

void ASlAiPlayerState::LoadState(float HPVal, float HungryVal)
{
	HP = HPVal;
	Hunger = HungryVal;

	//执行委托
	UpdateStateWidget.ExecuteIfBound(HP/500.f,Hunger/600.f);
}

void ASlAiPlayerState::SaveState(float& PlayerHP, float& PlayerHunger)
{
	PlayerHP = HP;
	PlayerHunger = Hunger;
}

void ASlAiPlayerState::BeginPlay()
{
	Super::BeginPlay();

	SPController = Cast<ASlAiPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
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
