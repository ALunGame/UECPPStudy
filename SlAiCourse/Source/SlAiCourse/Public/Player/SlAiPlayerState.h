// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "GameFramework/PlayerState.h"
#include "SlAiPlayerState.generated.h"


class ASlAiPlayerController;
DECLARE_DELEGATE_TwoParams(FUpdateStateWidget, float, float)

/**
 * 
 */
UCLASS()
class SLAICOURSE_API ASlAiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	//当前选中的物品Id
	int CurrShortcutIndex;

	//射线检测信息
	FText RayInfoText;

	//更新血量和饥饿
	FUpdateStateWidget UpdateStateWidget;

	ASlAiPlayerController* SPController;

public:
	ASlAiPlayerState();
	
	void OnRegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* Containers, TSharedPtr<STextBlock> ShortcutInfoTextBlock);

	virtual void Tick(float DeltaSeconds) override;

	//切换快捷栏
	void ChooseShortcut(bool IsUp);

	//获得当前选中的容器中物品Id
	int GetCurrHandleObjectIndex() const;

	//获得当前选中的容器中物品Id
	TSharedPtr<ObjectAttr> GetCurrHandleObjectAttr() const;

	//提供射线信息
	void RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock);

	//获得交互距离
	int GetAffectRange();

	//获得伤害值
	int GetDamageValue(EResourceType::Type ResourceType);

	//更改快捷栏信息
	void ChangeHandObject(int ShortcutID, int ObjectID, int ObjectNum);

	void PromoteHungry();

protected:
	virtual void BeginPlay() override;
	
private:

	//快捷栏序列
	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainers;
	//快捷栏信息参数
	TAttribute<FText> ShortcutInfoTextAttr;

	//射线信息
	TAttribute<FText> RayInfoTextAttr;

	//血量和饥饿
	float HP;
	float Hunger;

private:

	FText GetShortcutInfoText() const;

	FText GetRayInfoText() const;
};
