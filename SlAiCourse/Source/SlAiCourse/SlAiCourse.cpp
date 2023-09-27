// Fill out your copyright notice in the Description page of Project Settings.

#include "SlAiCourse.h"

#include "SlateStyleRegistry.h"
#include "Modules/ModuleManager.h"
#include "Style/SlAiStyle.h"


void FSlAiCourseModule::StartupModule()
{
	//初始化样式
	FSlateStyleRegistry::UnRegisterSlateStyle(SlAiStyle::GetStyleSetName());
	SlAiStyle::Init();
}

void FSlAiCourseModule::ShutdownModule()
{
	//清理样式
	SlAiStyle::ShutDown();
}

IMPLEMENT_PRIMARY_GAME_MODULE(FSlAiCourseModule, SlAiCourse, "SlAiCourse");


