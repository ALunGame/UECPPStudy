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
	
	void ResetMenuVolume(float MusicVol, float SoundVol);

public:
	//当前语言
	ECultureTeam CurCulture;
	//音量
	float MusicVolume;
	//音效
	float SoundVolume;
	//存档数据
	TArray<FString> RecordDataList;
	//存档名
	FString RecordName;

private:
	
	static TSharedRef<SlAiDataHandle> Create();

	template<typename T>
	FString EnumToString(T EnumValue);
	
	template<typename T>
	T StringToEnum(FString Value);

	void InitRecordData();

	//保存存档数据
	void SaveRecordData();

	
	void InitMenuAudio();

private:
	static TSharedPtr<SlAiDataHandle> DataHandleInstance;

	//保存Menu声音
	TMap<FString,TArray<USoundCue*>> MenuAudioResource;
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
};

