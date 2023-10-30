// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"
#include "GameFramework/PlayerState.h"
#include "SlAiPlayerState.generated.h"

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

public:
	ASlAiPlayerState();
	
	void OnRegisterShortcutContainer(TArray<TSharedPtr<ShortcutContainer>>* Containers, TSharedPtr<STextBlock> ShortcutInfoTextBlock);

	//切换快捷栏
	void ChooseShortcut(bool IsUp);

	//获得当前选中的容器中物品Id
	int GetCurrHandleObjectIndex() const;

	//获得当前选中的容器中物品Id
	TSharedPtr<ObjectAttr> GetCurrHandleObjectAttr() const;

	//提供射线信息
	void RegisterRayInfoEvent(TSharedPtr<STextBlock> RayInfoTextBlock);
	
private:

	//快捷栏序列
	TArray<TSharedPtr<ShortcutContainer>> ShortcutContainers;
	//快捷栏信息参数
	TAttribute<FText> ShortcutInfoTextAttr;

	//射线信息
	TAttribute<FText> RayInfoTextAttr;

private:

	FText GetShortcutInfoText() const;

	FText GetRayInfoText() const;
};
