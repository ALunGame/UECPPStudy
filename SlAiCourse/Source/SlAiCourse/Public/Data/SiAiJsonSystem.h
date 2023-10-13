// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstanceSubsystem.h"
#include "SiAiJsonSystem.generated.h"

/**
 * 
 */
UCLASS()
class SLAICOURSE_API USiAiJsonSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//解析存档
	void RecordDataJsonRead(FString& Culture, float& MusicVolume, float& SoundVolume, TArray<FString>& RecordDataList);

private:

	//读取Json
	bool LoadStringFromFile(const FString& FileName,const FString& RelaPath, FString& ResultString);
	
private:

	//存档文件名
	FString RecordDataFileName;

	//相对路径
	FString RelativePath;
};
