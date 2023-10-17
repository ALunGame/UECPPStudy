﻿// Fill out your copyright notice in the Description page of Project Settings.


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

void SlAiJsonHandle::UpdateRecordData(FString Culture, float MusicVolume, float SoundVolume,
	TArray<FString>* RecordDataList)
{
	TSharedPtr<FJsonObject> JsonObj = MakeShareable(new FJsonObject());

	TArray<TSharedPtr<FJsonValue>> BaseDataArray;

	TSharedPtr<FJsonObject> CultureObj = MakeShareable(new FJsonObject);
	CultureObj->SetStringField("Culture",Culture);
	TSharedPtr<FJsonValueObject> CultureValue = MakeShareable(new FJsonValueObject(CultureObj));
	BaseDataArray.Add(CultureValue);

	TSharedPtr<FJsonObject> MusicVolumeObj = MakeShareable(new FJsonObject);
	MusicVolumeObj->SetNumberField("MusicVolume",MusicVolume);
	TSharedPtr<FJsonValueObject> MusicVolumeValue = MakeShareable(new FJsonValueObject(MusicVolumeObj));
	BaseDataArray.Add(MusicVolumeValue);

	TSharedPtr<FJsonObject> SoundVolumeObj = MakeShareable(new FJsonObject);
	SoundVolumeObj->SetNumberField("SoundVolume",SoundVolume);
	TSharedPtr<FJsonValueObject> SoundVolumeValue = MakeShareable(new FJsonValueObject(SoundVolumeObj));
	BaseDataArray.Add(SoundVolumeValue);

	TArray<TSharedPtr<FJsonValue>> RecordDataArray;
	for (int i = 0; i < RecordDataList->Num(); ++i)
	{
		TSharedPtr<FJsonObject> recordObj = MakeShareable(new FJsonObject);
		recordObj->SetStringField(FString::FromInt(i),(*RecordDataList)[i]);
		TSharedPtr<FJsonValueObject> recordValue = MakeShareable(new FJsonValueObject(recordObj));
		RecordDataArray.Add(recordValue);
	}

	TSharedPtr<FJsonObject> RecordData = MakeShareable(new FJsonObject);
	RecordData->SetArrayField("RecordData",RecordDataArray);
	TSharedPtr<FJsonValueObject> RecordDataValue = MakeShareable(new FJsonValueObject(RecordData));
	BaseDataArray.Add(RecordDataValue);

	//转换为字符串
	FString JsonStr;
	GetFStringInJsonArrayData(BaseDataArray,JsonStr);
	// TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	// FJsonSerializer::Serialize(BaseDataArray,JsonWriter);
	SlAiHelper::Debug(FString("JsonStr>>>")+JsonStr,60.f);
	WriteFileWithJsonData(JsonStr,RelativePath,RecordDataFileName);
}

bool SlAiJsonHandle::LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString)
{
	if (!FileName.IsEmpty()) {
		//绝对路径
		FString AbsoPath = FPaths::ProjectContentDir() + RelaPath + FileName;
		if (FPaths::FileExists(AbsoPath)) {
			if (FFileHelper::LoadFileToString(ResultString, *AbsoPath))
			{
				return true;
			}
			else {
				SlAiHelper::Debug(FString("Load Error") + AbsoPath);
			}
		}
		else {
			SlAiHelper::Debug(FString("File Not Exist") + AbsoPath);
		}
	}
	return false;
}

bool SlAiJsonHandle::GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr)
{
	if (!JsonObj.IsValid() || JsonObj->Values.Num() <= 0)
	{
		return false;
	}
	
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), JsonWriter);
	
	return true;
}

bool SlAiJsonHandle::GetFStringInJsonArrayData(const TArray<TSharedPtr<FJsonValue>>& JsonList, FString& JsonStr)
{
	if (JsonList.IsEmpty())
	{
		return false;
	}
	
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonList, JsonWriter);
	
	return true;
}

bool SlAiJsonHandle::WriteFileWithJsonData(const FString& JsonStr, const FString& RelativePath,
                                                  const FString& FileName)
{
	if (JsonStr.IsEmpty() || RelativePath.IsEmpty() || FileName.IsEmpty())
	{
		return false;
	}

	FString AbsoPath = FPaths::ProjectContentDir() + RelativePath + FileName;

	if (FFileHelper::SaveStringToFile(JsonStr,*AbsoPath))
	{
		return true;
	}
	SlAiHelper::DebugError("存档保存失败！！"+AbsoPath,10.f);
	return false;
}

