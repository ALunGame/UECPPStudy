// Fill out your copyright notice in the Description page of Project Settings.


#include "SiAiJsonSystem.h"

#include "FileHelper.h"
#include "JsonSerializer.h"
#include "SlAiHelper.h"

void USiAiJsonSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RecordDataFileName = FString("RecordData.json");
	RelativePath = FString("Res/ConfigData/");
}

void USiAiJsonSystem::Deinitialize()
{
	Super::Deinitialize();
}

void USiAiJsonSystem::RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume,
	TArray<FString>& RecordDataList)
{
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName,RelativePath,JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
	
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		//获取数据
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));

		//获取存档数据
		TArray<TSharedPtr<FJsonValue>> RecordDataArray = JsonParsed[3]->AsObject()->GetArrayField(FString("RecordData"));
		for (int i = 0; i < RecordDataArray.Num(); ++i)
		{
			FString RecordDataName = RecordDataArray[i]->AsObject()->GetStringField(FString::FromInt(i));
			RecordDataList.Add(RecordDataName);
		}
	}
	else
	{
		SlAiHelper::DebugError(FString("存档解析失败！！"));
	}
}

bool USiAiJsonSystem::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (!FileName.IsEmpty())
	{
		//绝对路径
		FString AbsoPath = FPaths::ProjectContentDir() + RelativePath + FileName;
		
		if (FPaths::FileExists(AbsoPath))
		{
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath))
			{
				return true;
			}
			SlAiHelper::DebugError(FString("加载存档文件失败！！") + AbsoPath);
		}
		else
		{
			SlAiHelper::DebugError(FString("存档文件不存在！！") + AbsoPath);
		}
	}
	return false;
}
