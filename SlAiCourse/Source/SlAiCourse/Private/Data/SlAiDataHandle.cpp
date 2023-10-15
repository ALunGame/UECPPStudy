// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SlAiDataHandle.h"

#include "SiAiJsonSystem.h"
#include "SlAiHelper.h"
#include "SlAiJsonHandle.h"
#include "SlAiSingleton.h"
#include "Engine/GameEngine.h"
#include "Helper/SlAiGetter.h"
#include "Kismet/GameplayStatics.h"

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

	//输出
	// SlAiHelper::Debug(Culture + FString(">>") + FString::SanitizeFloat(MusicVolume) + FString(">>") + FString::SanitizeFloat(SoundVolume),20.f);
	// for (TArray<FString>::TIterator It(RecordDataList); It; ++It)
	// {
	// 	SlAiHelper::Debug(*It,20.f);
	// }
	
	//测试
	// SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(Culture, MusicVolume, SoundVolume, new TArray<FString>
	// {
	// 	"1111",
	// });
}

void SlAiDataHandle::SaveRecordData()
{
	SlAiSingleton<SlAiJsonHandle>::Get()->UpdateRecordData(EnumToString(CurCulture), MusicVolume, SoundVolume, &RecordDataList);
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

void SlAiDataHandle::SetMusicVolume(float newValue)
{
	MusicVolume = newValue;
	SaveRecordData();
}

void SlAiDataHandle::SetSoundVolume(float newValue)
{
	SoundVolume = newValue;
	SaveRecordData();
}

SlAiDataHandle::SlAiDataHandle()
{
	CurCulture = ECultureTeam::ZH;

	MusicVolume = 0.5f;
	SoundVolume = 0.5f;

	//初始化存档
	InitRecordData();
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


