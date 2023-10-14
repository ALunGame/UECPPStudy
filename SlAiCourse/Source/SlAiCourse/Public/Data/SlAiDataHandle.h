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

	void ChangeLocalizationCulture(ECultureTeam newCulture);

	void SetMusicVolume(float newValue);
	
	void SetSoundVolume(float newValue);

public:
	//当前语言
	ECultureTeam CurCulture;
	//音量
	float MusicVolume;
	//音效
	float SoundVolume;
	//存档数据
	TArray<FString> RecordDataList;

private:
	
	static TSharedRef<SlAiDataHandle> Create();

	template<typename T>
	FString EnumToString(T EnumValue);
	
	template<typename T>
	T StringToEnum(FString Value);

	void InitRecordData();

	//保存存档数据
	void SaveRecordData();
	
	static TSharedPtr<SlAiDataHandle> DataHandleInstance;
};

