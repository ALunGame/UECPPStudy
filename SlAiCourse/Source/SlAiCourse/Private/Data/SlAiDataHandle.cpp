// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SlAiDataHandle.h"

#include "SiAiJsonSystem.h"
#include "SlAiHelper.h"
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
	//获取语言
	FString Culture;

	//读取存档
	UGameInstance* GameInstance = SlAiGetter::GetGameInstance();
	USiAiJsonSystem* JsonSystem = GameInstance->GetSubsystem<USiAiJsonSystem>();
	JsonSystem->RecordDataJsonRead(Culture,MusicVolume,SoundVolume,RecordDataList);

	//初始化语言
	ChangeLocalizationCulture(StringToEnum<ECultureTeam>(Culture));
	
	
	//输出
	SlAiHelper::Debug(Culture + FString(">>") + FString::SanitizeFloat(MusicVolume) + FString(">>") + FString::SanitizeFloat(SoundVolume),20.f);
	for (TArray<FString>::TIterator It(RecordDataList); It; ++It)
	{
		SlAiHelper::Debug(*It,20.f);
	}
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
}

void SlAiDataHandle::SetMusicVolume(float newValue)
{
	MusicVolume = newValue;
}

void SlAiDataHandle::SetSoundVolume(float newValue)
{
	SoundVolume = newValue;
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


