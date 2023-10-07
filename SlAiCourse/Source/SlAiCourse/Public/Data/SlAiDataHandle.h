// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlAiTypes.h"

/**
 * 
 */
class SLAICOURSE_API SlAiDataHandle
{
public:
	SlAiDataHandle();
	
	static TSharedPtr<SlAiDataHandle> Get();
	
	static void Init();

	void ChangeLocalizationCulTure(ECultrueTeam newCulture);

public:
	//当前语言
	ECultrueTeam CurCultrue;

private:
	static TSharedRef<SlAiDataHandle> Create();
	static TSharedPtr<SlAiDataHandle> DataHandleInstance;
};
