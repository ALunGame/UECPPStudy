// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileHelper.h"
#include "SlAiTypes.h"

struct ObjectAttr;
/**
 * 
 */
class SLAICOURSE_API SlAiJsonHandle
{
public:
	SlAiJsonHandle();

	//解析存档
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);

	//更新存档
	void UpdateRecordData(FString Culture,float MusicVolume, float SoundVolume,TArray<FString>* RecordDataList);

	//解析物品属性
	void ObjectAttrJsonRead(TMap<int, TSharedPtr<ObjectAttr>>& ObjectAttrMap);

	//解析资源属性
	void ResourceAttrJsonRead(TMap<int, TSharedPtr<ResourceAttribute>>& ResourceAttrMap);
	
private:

	//读取Json文件到字符串
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	//FJsonObject转化为Json格式字符串
	bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

	//FJsonArray转化为Json格式字符串
	bool GetFStringInJsonArrayData(const TArray<TSharedPtr<FJsonValue>>& JsonList, FString& JsonStr);
	
	//保存字符串到文件
	bool WriteFileWithJsonData(const FString& JsonStr, const FString& RelativePath, const FString& FileName);

	//字符串转物品类型
	EObjectType::Type StringToObjectType(const FString ArgStr);
	EResourceType::Type StringToResourceType(FString ArgStr);

private:
	//存档文件名
	FString RecordDataFileName;

	//物品文件名
	FString ObjectAttrFileName;

	//资源文件名
	FString ResourceAttrFileName;
	
	//文件相对路径
	FString RelativePath;
};


