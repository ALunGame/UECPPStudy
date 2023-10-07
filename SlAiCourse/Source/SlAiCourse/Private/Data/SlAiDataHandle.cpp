// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/SlAiDataHandle.h"

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

void SlAiDataHandle::ChangeLocalizationCulTure(ECultrueTeam newCulture)
{
	switch (newCulture)
	{
		case ECultrueTeam::EN:
			FInternationalization::Get().SetCurrentCulture(TEXT("en"));
			break;
		case ECultrueTeam::ZH:
			FInternationalization::Get().SetCurrentCulture(TEXT("zh"));
			break;
	}
	CurCultrue = newCulture;
}

SlAiDataHandle::SlAiDataHandle()
{
}



