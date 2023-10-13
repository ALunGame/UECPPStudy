// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SLAICOURSE_API SlAiJsonHandle
{
public:
	SlAiJsonHandle();

	//读取存档数据
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);

private:

	//
	bool LoadStringFromFile(const FString& FileName, const FString& RelaPath, FString& ResultString);

	//
	//bool GetFStringInJsonData(const TSharedPtr<FJsonObject>& JsonObj, FString& JsonStr);

private:
	//存档文件名
	FString RecordDataFileName;

	//文件相对路径
	FString RelativePath;
};
