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
	
	void ResetGameVolume(float MusicVol, float SoundVol);

	void InitGameData();
	void AddNewRecord();

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
	//物品属性
	TMap<int,TSharedPtr<ObjectAttr>> ObjectAttrMap;
	//物品贴图
	TArray<const FSlateBrush*> ObjectBrushList;

	//资源属性
	TMap<int,TSharedPtr<ResourceAttribute>> ResourceAttrMap;

	//合成表
	TArray<TSharedPtr<CompoundTable>> CompoundTableMap;

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

	void InitObjectAttr();

	void InitResourceAttr();
	
	void InitCompoundTableMap();

	void InitGameAudio();
private:
	static TSharedPtr<SlAiDataHandle> DataHandleInstance;

	//保存Menu声音
	TMap<FString,TArray<USoundCue*>> MenuAudioResource;
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	//获取Menu样式
	const struct FSlAiGameStyle *GameStyle;

	//音乐组件
	class USoundMix* SlAiSoundMix;
	
	class USoundClass* SlAiSoundClass;
	class USoundClass* SlAiMusicClass;

	class FAudioDevice* AudioDevice;
};

