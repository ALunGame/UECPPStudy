﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiJsonHandle.h"

#include "FileHelper.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "SlAiHelper.h"
#include "SlAiTypes.h"


SlAiJsonHandle::SlAiJsonHandle()
{
	RecordDataFileName = FString("RecordData.json");

	ObjectAttrFileName = FString("ObjectAttribute.json");
	
	ResourceAttrFileName = FString("ResourceAttribute.json");
	
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
	WriteFileWithJsonData(JsonStr,RelativePath,RecordDataFileName);
}

void SlAiJsonHandle::ObjectAttrJsonRead(TMap<int, TSharedPtr<ObjectAttr>>& ObjectAttrMap)
{
	FString JsonValue;
	LoadStringFromFile(ObjectAttrFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		for (int i = 0; i < JsonParsed.Num(); ++i) {
			TArray<TSharedPtr<FJsonValue>> ObjectValue = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i));
			FText EN = FText::FromString(ObjectValue[0]->AsObject()->GetStringField("EN"));
			FText ZH = FText::FromString(ObjectValue[1]->AsObject()->GetStringField("ZH"));
			FString ObjectTypeStr = ObjectValue[2]->AsObject()->GetStringField("ObjectType");
			int PlantAttack = ObjectValue[3]->AsObject()->GetIntegerField("PlantAttack");
			int MetalAttcck = ObjectValue[4]->AsObject()->GetIntegerField("MetalAttcck");
			int AnimalAttack = ObjectValue[5]->AsObject()->GetIntegerField("AnimalAttack");
			int AffectRange = ObjectValue[6]->AsObject()->GetIntegerField("AffectRange");
			FString TexPath = ObjectValue[7]->AsObject()->GetStringField("TexPath");

			EObjectType::Type ObjectType = StringToObjectType(ObjectTypeStr);
			TSharedPtr<ObjectAttr> ObjectAttrPtr = MakeShareable(new ObjectAttr(EN, ZH, ObjectType, PlantAttack, MetalAttcck, AnimalAttack, AffectRange, TexPath));

			ObjectAttrMap.Add(i, ObjectAttrPtr);
		}
	}
	else {
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
}

void SlAiJsonHandle::ResourceAttrJsonRead(TMap<int, TSharedPtr<ResourceAttribute>>& ResourceAttrMap)
{
	FString JsonValue;
	LoadStringFromFile(ResourceAttrFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {
		for (int i = 0; i < JsonParsed.Num(); ++i) {
			TArray<TSharedPtr<FJsonValue>> ObjectValue = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i+1));
			FText EN = FText::FromString(ObjectValue[0]->AsObject()->GetStringField("EN"));
			FText ZH = FText::FromString(ObjectValue[1]->AsObject()->GetStringField("ZH"));
			FString ResourceTypeStr = ObjectValue[2]->AsObject()->GetStringField("ResourceType");
			int HP = ObjectValue[3]->AsObject()->GetIntegerField("HP");
			
			TArray<TArray<int>> FlobObjectInfoArray;

			TArray<TSharedPtr<FJsonValue>> FlobObjectInfo = ObjectValue[4]->AsObject()->GetArrayField(FString("FlobObjectInfo"));

			for (int j = 0; j < FlobObjectInfo.Num(); ++j) {

				FString FlobObjectInfoItem = FlobObjectInfo[j]->AsObject()->GetStringField(FString::FromInt(j));
				FString ObjectIndexStr;
				FString RangeStr;
				FString RangeMinStr;
				FString RangeMaxStr;
				FlobObjectInfoItem.Split(FString("_"), &ObjectIndexStr, &RangeStr);
				RangeStr.Split(FString(","), &RangeMinStr, &RangeMaxStr);

				TArray<int> FlobObjectInfoList;

				//第一位是物品Id，后俩位是最小最大数量
				FlobObjectInfoList.Add(FCString::Atoi(*ObjectIndexStr));
				FlobObjectInfoList.Add(FCString::Atoi(*RangeMinStr));
				FlobObjectInfoList.Add(FCString::Atoi(*RangeMaxStr));

				FlobObjectInfoArray.Add(FlobObjectInfoList);
			}

			EResourceType::Type ResourceType = StringToResourceType(ResourceTypeStr);
			TSharedPtr<ResourceAttribute> ResourceAttrPtr = MakeShareable(new ResourceAttribute(EN, ZH, ResourceType, HP, &FlobObjectInfoArray));

			ResourceAttrMap.Add(i + 1, ResourceAttrPtr);
		}
	}
	else {
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
}

void SlAiJsonHandle::CompoundTableJsonRead(TArray<TSharedPtr<CompoundTable>>& CompoundTableMap)
{
	FString JsonValue;
	LoadStringFromFile(CompoundTableFileName, RelativePath, JsonValue);

	TArray<TSharedPtr<FJsonValue>> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);

	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{

		for (int i = 0; i < JsonParsed.Num(); ++i) {
			TArray<TSharedPtr<FJsonValue>> ObjectAttr = JsonParsed[i]->AsObject()->GetArrayField(FString::FromInt(i));

			TArray<int> CompoundTableArr;

			for (int j = 0; j < 10; ++j) {
				CompoundTableArr.Add(ObjectAttr[j]->AsObject()->GetIntegerField(FString::FromInt(j)));
			}

			TSharedPtr<CompoundTable> NewTable = MakeShareable(new CompoundTable(&CompoundTableArr));

			CompoundTableMap.Add(NewTable);

		}

	}
	else {
		SlAiHelper::Debug(FString("Deserialize Failed"));
	}
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

EObjectType::Type SlAiJsonHandle::StringToObjectType(const FString ArgStr)
{
	if (ArgStr.Equals(FString("Normal"))) return EObjectType::Normal;
	if (ArgStr.Equals(FString("Food"))) return EObjectType::Food;
	if (ArgStr.Equals(FString("Tool"))) return EObjectType::Tool;
	if (ArgStr.Equals(FString("Weapon"))) return EObjectType::Weapon;
	return EObjectType::Normal;
}

EResourceType::Type SlAiJsonHandle::StringToResourceType(const FString ArgStr)
{
	if (ArgStr.Equals(FString("Plant"))) return EResourceType::Plant;
	if (ArgStr.Equals(FString("Metal"))) return EResourceType::Metal;
	if (ArgStr.Equals(FString("Animal"))) return EResourceType::Animal;
	return EResourceType::Plant;
}

