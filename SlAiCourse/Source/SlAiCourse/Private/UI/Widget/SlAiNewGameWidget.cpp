// Fill out your copyright notice in the Description page of Project Settings.


#include "SlAiNewGameWidget.h"

#include "SlAiDataHandle.h"
#include "SlAiMenuWidgetStyle.h"
#include "SlAiStyle.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SlAiNewGameWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");
	
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
					SAssignNew(EditableTextBox,SEditableTextBox)
					.HintText(NSLOCTEXT("SlAiMenu","RecordNameHint","Input Record Name!"))
					.Font(MenuStyle->Font_30)
				]
				
			]
		]
	]; 
}

bool SlAiNewGameWidget::AllowEnterGame()
{
	//获得输入存档
	FText InputText = EditableTextBox->GetText();
	if (InputText.ToString().IsEmpty())
	{
		return false;
	}

	//查询是否重名
	for (auto Record : SlAiDataHandle::Get()->RecordDataList)
	{
		if (Record.Equals(InputText.ToString()))
		{
			EditableTextBox->SetText(FText::FromString(""));
			EditableTextBox->SetHintText(NSLOCTEXT("SlAiMenu","NameRepeatedHint","RecordName Name Repeated"));
			return false;
		}
	}

	//保存新存档
	SlAiDataHandle::Get()->RecordName = InputText.ToString();
	
	return true;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
