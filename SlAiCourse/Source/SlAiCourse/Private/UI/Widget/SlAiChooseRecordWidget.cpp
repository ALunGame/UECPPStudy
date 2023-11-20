// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiChooseRecordWidget.h"

#include "SlAiDataHandle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"
#include "STextComboBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiChooseRecordWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//初始化下拉菜单
	for (FString Record : SlAiDataHandle::Get()->RecordDataList)
	{
		OptionSource.Add(MakeShareable(new FString(*Record)));
	}

	if (OptionSource.Num() <= 0)
	{
		return;
	}
	
	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(500.f)
		.HeightOverride(100.f)
		[
			SNew(SOverlay)

			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuItemBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(20.f,0.f,0.f,0.f))
			[
				SNew(STextBlock)
				.Font(MenuStyle->Font_40)
				.Text(NSLOCTEXT("SlAiMenu","NewGame","NewGame"))
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0.f,0.f,20.f,0.f))
			[
				SNew(SBox)
				.WidthOverride(300.f)
				.HeightOverride(60.f)
				[
					SAssignNew(RecordComboBox,STextComboBox)
					.Font(MenuStyle->Font_30)
					.OptionsSource(&OptionSource)
				]
			]
		]
	];

	//设置默认选择
	RecordComboBox->SetSelectedItem(OptionSource[0]);

}

void SlAiChooseRecordWidget::UpdateRecordName()
{
	//修改存档名
	SlAiDataHandle::Get()->RecordName = *RecordComboBox->GetSelectedItem().Get();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
