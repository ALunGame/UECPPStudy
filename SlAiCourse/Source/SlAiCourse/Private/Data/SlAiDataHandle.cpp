// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SlAiDataHandle.h"

#include "SiAiJsonSystem.h"
#include "SlAiHelper.h"
#include "SlAiJsonHandle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiSingleton.h"
#include "SlAiStyle.h"
#include "Engine/GameEngine.h"
#include "Helper/SlAiGetter.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

TSharedPtr<SlAiDataHandle> SlAiDataHandle::DataHandleInstance = nullptr;


void SlAiDataHandle::Init()
{
	if (!DataHandleInstance.IsValid())
	{
		DataHandleInstance = Create();
	}
}

TSharedPtr<SlAiDataHandle> SlAiDataHandle::Get()
{
	Init();
	return DataHandleInstance;
}

TSharedRef<SlAiDataHandle> SlAiDataHandle::Create()
{
	TSharedRef<SlAiDataHandle> dataRef = MakeShareable(new SlAiDataHandle());
	return dataRef;
}

void SlAiDataHandle::InitRecordData()
{
	RecordName = FString("");
	
	//获取语言
	FString Culture;

	//读取存档
	SlAiSingleton<SlAiJsonHandle>::Get()->RecordDataJsonRead(Culture, MusicVolume, SoundVolume, RecordDataList);

	//初始化语言
	ChangeLocalizationCulture(StringToEnum<ECultureTeam>(Culture));
}

void SlAiDataHandle::SaveRecordData()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(EnumToString(CurCulture), MusicVolume, SoundVolume, &RecordDataList);
}

void SlAiDataHandle::InitMenuAudio()
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	TArray<USoundCue*> MusicList;
	MusicList.Add(Cast<USoundCue>(MenuStyle->MenuBackgroundMusic.GetResourceObject()));

	TArray<USoundCue*> SoundList;
	SoundList.Add(Cast<USoundCue>(MenuStyle->StartGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->ExitGameSound.GetResourceObject()));
	SoundList.Add(Cast<USoundCue>(MenuStyle->MenuItemChangeSound.GetResourceObject()));

	MenuAudioResource.Add(FString("Music"),MusicList);
	MenuAudioResource.Add(FString("Sound"),SoundList);

	ResetMenuVolume(MusicVolume, SoundVolume);
}

void SlAiDataHandle::InitObjectAttr()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->ObjectAttrJsonRead(ObjectAttrMap);
}

void SlAiDataHandle::ChangeLocalizationCulture(ECultureTeam newCulture)
{
	switch (newCulture)
	{
		case ECultureTeam::EN:
			FInternationalization::Get().SetCurrentCulture(TEXT("en"));
			break;
		case ECultureTeam::ZH:
			FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
			break;
	}
	CurCulture = newCulture;
	SaveRecordData();
}

void SlAiDataHandle::ResetMenuVolume(float MusicVol, float SoundVol)
{
	if (MusicVol > 0)
	{
		MusicVolume = MusicVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Music"))->CreateIterator()); It; ++It)
		{
			//音量
			(*It)->VolumeMultiplier = MusicVolume;
		}
	}
	if (SoundVol > 0)
	{
		SoundVolume = SoundVol;
		for (TArray<USoundCue*>::TIterator It(MenuAudioResource.Find(FString("Sound"))->CreateIterator()); It; ++It)
		{
			//音量
			(*It)->VolumeMultiplier = SoundVolume;
		}
	}

	SaveRecordData();
}

void SlAiDataHandle::InitGameData()
{
	InitObjectAttr();
}

SlAiDataHandle::SlAiDataHandle()
{
	CurCulture = ECultureTeam::ZH;

	MusicVolume = 0.5f;
	SoundVolume = 0.5f;

	//初始化存档
	InitRecordData();

	//初始化菜单
	InitMenuAudio();
}

template <typename T>
FString SlAiDataHandle::EnumToString(T EnumValue)
{
	//static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum" );
	return StaticEnum<T>()->GetNameStringByIndex((int32)EnumValue);
}

template <typename T>
T SlAiDataHandle::StringToEnum(FString Value)
{
	//static_assert(TIsUEnumClass<T>::Value, "'T' template parameter to EnumToString must be a valid UEnum" );
	return (T)StaticEnum<T>()->GetValueByNameString(Value);
}


