// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiJsonHandle.h"

#include "FileHelper.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "SlAiHelper.h"


SlAiJsonHandle::SlAiJsonHandle()
{
	RecordDataFileName = FString("RecordData.json");
	
	RelativePath = FString("Res/ConfigData/");
}

void SlAiJsonHandle::RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume,
	TArray<FString>& RecordDataList)
{
	FString JsonValue;
	LoadStringFromFile(RecordDataFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		//存档数据
		Culture = JsonParsed[0]->AsObject()->GetStringField(FString("Culture"));
		MusicVolume = JsonParsed[1]->AsObject()->GetNumberField(FString("MusicVolume"));
		SoundVolume = JsonParsed[2]->AsObject()->GetNumberField(FString("SoundVolume"));
		
		//存档列表
		TArray<TSharedPtr<FJsonValue>> RecordDataArray = JsonParsed[3]->AsObject()->GetArrayField(FString("RecordData"));
		for (int i = 0; i < RecordDataArray.Num(); ++i) {
			FString RecordDataName = RecordDataArray[i]->AsObject()->GetStringField(FString::FromInt(i));
			RecordDataList.Add(RecordDataName);
		}
	}
	else {
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
}

bool SlAiJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (!FileName.IsEmpty()) {
		//��ȡ����·��
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		if (FPaths::FileExists(AbsoPath)) {
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath))
			{
				return true;
			}
			else {
				//���ز��ɹ�
				SlAiHelper::Debug(FString("Load Error") + AbsoPath);
			}
		}
		else {
			//����ļ�������
			SlAiHelper::Debug(FString("File Not Exist") + AbsoPath);
		}
	}
	return false;
}

// bool SlAiJsonHandle::GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr)
// {
// }

