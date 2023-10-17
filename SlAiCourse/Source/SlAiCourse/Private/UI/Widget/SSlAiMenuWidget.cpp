// Fill out your copyright notice in the Description page of Project Settings.


#include "SSlAiMenuWidget.h"

#include "SlAiChooseRecordWidget.h"
#include "SlAiDataHandle.h"

#include "SlAiMenuWidgetStyle.h"
#include "SlAiNewGameWidget.h"
#include "SlAiStyle.h"
#include "SlAiTypes.h"
#include "SlateOptMacros.h"
#include "Common/SlAiHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

struct MenuGroup
{
	//菜单标题
	FText MenuName;

	//菜单高度
	float MenuHeight;

	//下属组件
	TArray<TSharedPtr<SCompoundWidget>> ChildWidget;

	//构造
	MenuGroup(const FText Name, const float Height, TArray<TSharedPtr<SCompoundWidget>>* Children)
	{
		MenuName = Name;
		MenuHeight = Height;
		for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It(*Children); It; ++It)
		{
			ChildWidget.Add(*It);
		}
	}
};

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlAiMenuWidget::Construct(const FArguments& InArgs)
{
	//获取编辑器的MenuStyle
	MenuStyle = &SlAiStyle::Get().GetWidgetStyle<FSlAiMenuStyle>("BPSlAiMenuStyle");

	//播放背景音乐
	FSlateApplication::Get().PlaySound(MenuStyle->MenuBackgroundMusic);
	
	ChildSlot
	[
		SAssignNew(RootSizeBox,SBox)
		[
			SNew(SOverlay)
			
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Padding(FMargin(0,50.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->MenuBackgroundBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0,25.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->LeftIconBrush)
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(FMargin(0,25.f,0,0))
			[
				SNew(SImage)
				.Image(&MenuStyle->RightIconBrush)
			]
			
			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SBox)
				.WidthOverride(400.f).HeightOverride(100.f)
				[
					SNew(SBorder)
					.BorderImage(&MenuStyle->TitleBorderBrush)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SAssignNew(TitleText,STextBlock)
						.Font(MenuStyle->FontInfo)
						.Text(NSLOCTEXT("SlAiMenu","Menu","Menu"))
					]
				]
			]

			+SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.Padding(FMargin(0.f,130.f,0.f,0.f))
			[
				SAssignNew(ContentBox,SVerticalBox)
			]
		]
		
	];
	
	InitMenuList();
	InitAnimation();
}

void SSlAiMenuWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	switch (AnimState)
	{
	case EMenuAnim::Stop:
		break;
	case EMenuAnim::Close:
		if (MenuAnimation.IsPlaying())
		{
			//修改高度
			ResetWidgetSize(MenuCurve.GetLerp()*600.f,-1.f);
			//在关闭了40% 关闭
			if (MenuCurve.GetLerp() < 0.6f)
			{
				ChooseWidget(EMenuType::None);
			}
		}
		else
		{
			AnimState = EMenuAnim::Open;
			MenuAnimation.Play(this->AsShared());
		}
		break;
	case EMenuAnim::Open:
		if (MenuAnimation.IsPlaying())
		{
			//修改高度
			ResetWidgetSize(MenuCurve.GetLerp()*600.f,CurrHeight);
			//在打开了60% 显示
			if (MenuCurve.GetLerp() > 0.6f && !IsMenuShow)
			{
				ChooseWidget(CurrMenu);
			}
		}
		if (MenuAnimation.IsAtEnd())
		{
			AnimState = EMenuAnim::Stop;
			ControlLocked = false;
		}
		break;
	default: ;
	}
}

void SSlAiMenuWidget::MenuItemOnClicked(EMenuItem::Type ItemType)
{
	if (ControlLocked)
	{
		return;
	}
	ControlLocked = true;

	switch (ItemType)
	{
	case EMenuItem::None:
		ControlLocked = false;
		break;
	case EMenuItem::StartGame:
		PlayCloseAnim(EMenuType::StartGame);
		break;
	case EMenuItem::GameOption:
		PlayCloseAnim(EMenuType::GameOption);
		break;
	case EMenuItem::QuitGame:
		ControlLocked = false;
		SlAiHelper::PlaySoundAndCall(UGameplayStatics::GetPlayerController(GWorld,0)->GetWorld(),MenuStyle->ExitGameSound,this,&SSlAiMenuWidget::QuitGame);
		break;
	case EMenuItem::NewGame:
		PlayCloseAnim(EMenuType::NewGame);
		break;
	case EMenuItem::LoadRecord:
		PlayCloseAnim(EMenuType::ChooseRecord);
		break;
	case EMenuItem::StartGameGoBack:
		PlayCloseAnim(EMenuType::MainMenu);
		break;
	case EMenuItem::GameOptionGoBack:
		PlayCloseAnim(EMenuType::MainMenu);
		break;
	case EMenuItem::NewGameGoBack:
		PlayCloseAnim(EMenuType::StartGame);
		break;
	case EMenuItem::ChooseRecordGoBack:
		PlayCloseAnim(EMenuType::StartGame);
		break;
	case EMenuItem::EnterGame:
		if (NewGameWidget->AllowEnterGame())
		{
			SlAiHelper::PlaySoundAndCall(UGameplayStatics::GetPlayerController(GWorld,0)->GetWorld(),MenuStyle->StartGameSound,this,&SSlAiMenuWidget::EnterGame);
		}
		else
		{
			ControlLocked = false;
		}
		break;
	case EMenuItem::EnterRecord:
		ChooseRecordWidget->UpdateRecordName();
		ControlLocked = false;
		SlAiHelper::PlaySoundAndCall(UGameplayStatics::GetPlayerController(GWorld,0)->GetWorld(),MenuStyle->StartGameSound,this,&SSlAiMenuWidget::EnterGame);
		break;
	default: ;
	}
}

void SSlAiMenuWidget::OnChangeCulture(ECultureTeam NewCulture)
{
	SlAiDataHandle::Get()->ChangeLocalizationCulture(NewCulture);
}

void SSlAiMenuWidget::OnChangeVolume(float MusicVolume, float SoundVolume)
{
	SlAiDataHandle::Get()->ResetMenuVolume(MusicVolume,SoundVolume);
}

void SSlAiMenuWidget::InitMenuList()
{
	//主界面
	TArray<TSharedPtr<SCompoundWidget>> MainMenuList;
	MainMenuList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","StartGame","StartGame"))
		.ItemType(EMenuItem::StartGame)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	MainMenuList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","GameOption","GameOption"))
		.ItemType(EMenuItem::GameOption)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	MainMenuList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","QuitGame","QuitGame"))
		.ItemType(EMenuItem::QuitGame)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::MainMenu, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "Menu", "Menu"), 510.f, &MainMenuList)));

	//开始游戏
	TArray<TSharedPtr<SCompoundWidget>> StartGameList;
	StartGameList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","NewGame","NewGame"))
		.ItemType(EMenuItem::NewGame)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	StartGameList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","LoadRecord","LoadRecord"))
		.ItemType(EMenuItem::LoadRecord)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	StartGameList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","GoBack","GoBack"))
		.ItemType(EMenuItem::StartGameGoBack)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));
	
	MenuMap.Add(EMenuType::StartGame, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "StartGame", "StartGame"), 510.f, &StartGameList)));

	//游戏设置
	TArray<TSharedPtr<SCompoundWidget>> GameOptionList;
	SAssignNew(GameOptionWidget, SlAiGameOptionWidget)
	.OnChangeCulture(this,&SSlAiMenuWidget::OnChangeCulture)
	.OnChangeVolume(this,&SSlAiMenuWidget::OnChangeVolume);
	GameOptionList.Add(GameOptionWidget);
	
	GameOptionList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","GoBack","GoBack"))
		.ItemType(EMenuItem::GameOptionGoBack)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::GameOption, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "GameOption", "GameOption"), 610.f, &GameOptionList)));

	//开始游戏
	TArray<TSharedPtr<SCompoundWidget>> NewGameList;
	SAssignNew(NewGameWidget, SlAiNewGameWidget);
	NewGameList.Add(NewGameWidget);

	NewGameList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","EnterGame","EnterGame"))
		.ItemType(EMenuItem::EnterGame)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	NewGameList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","GoBack","GoBack"))
		.ItemType(EMenuItem::NewGameGoBack)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	MenuMap.Add(EMenuType::NewGame, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "NewGame", "NewGame"), 510.f, &NewGameList)));

	//选择存档
	TArray<TSharedPtr<SCompoundWidget>> ChooseRecordList;
	SAssignNew(ChooseRecordWidget, SlAiChooseRecordWidget);
	ChooseRecordList.Add(ChooseRecordWidget);

	ChooseRecordList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","EnterRecord","EnterRecord"))
		.ItemType(EMenuItem::EnterRecord)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));

	ChooseRecordList.Add(SNew(SlAiMenuItemWidget)
		.ItemText(NSLOCTEXT("SlAiMenu","GoBack","GoBack"))
		.ItemType(EMenuItem::ChooseRecordGoBack)
		.OnClicked(this,&SSlAiMenuWidget::MenuItemOnClicked));


	MenuMap.Add(EMenuType::ChooseRecord, MakeShareable(new MenuGroup(NSLOCTEXT("SlAiMenu", "LoadRecord", "LoadRecord"), 510.f, &ChooseRecordList)));
}

void SSlAiMenuWidget::ChooseWidget(EMenuType::Type WidgetType)
{
	IsMenuShow = WidgetType != EMenuType::None;
	
	//移除所有组件
	ContentBox->ClearChildren();

	if (WidgetType == EMenuType::None)
		return;

	//添加组件
	for (TArray<TSharedPtr<SCompoundWidget>>::TIterator It((*MenuMap.Find(WidgetType))->ChildWidget); It; ++It)
	{
		ContentBox->AddSlot().AutoHeight()[(*It)->AsShared()];
	}

	//更改标题
	TitleText->SetText((*MenuMap.Find(WidgetType))->MenuName);
}

void SSlAiMenuWidget::ResetWidgetSize(float NewWidth, float NewHeight)
{
	RootSizeBox->SetWidthOverride(NewWidth);
	if (NewHeight > 0)
		RootSizeBox->SetHeightOverride(NewHeight);
}

void SSlAiMenuWidget::InitAnimation()
{
	//开始延时
	const float StartDelay = 0.3f;
	const float AnimDuration = 0.6f;
	
	MenuAnimation = FCurveSequence();
	MenuCurve = MenuAnimation.AddCurve(StartDelay,AnimDuration,ECurveEaseFunction::QuadInOut);

	//初始化大小
	ResetWidgetSize(600.f,510.f);
	ChooseWidget(EMenuType::MainMenu);

	//允许点击
	ControlLocked = false;

	//状态
	AnimState = EMenuAnim::Stop;

	//设置到结尾
	MenuAnimation.JumpToEnd();
}

void SSlAiMenuWidget::PlayCloseAnim(EMenuType::Type NewMenu)
{
	//设置新界面
	CurrMenu = NewMenu;

	//新高度
	CurrHeight = (*MenuMap.Find(NewMenu))->MenuHeight;

	//状态
	AnimState = EMenuAnim::Close;

	//反向播放
	MenuAnimation.PlayReverse(this->AsShared());

	//音效
	FSlateApplication::Get().PlaySound(MenuStyle->MenuItemChangeSound);

}

void SSlAiMenuWidget::QuitGame()
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GWorld,0);
	UKismetSystemLibrary::QuitGame(GWorld,PlayerController,EQuitPreference::Quit,false);
}

void SSlAiMenuWidget::EnterGame()
{
	SlAiHelper::Debug(FString("EnterGame！！！！"),10.f);

	UGameplayStatics::OpenLevel(UGameplayStatics::GetPlayerController(GWorld,0)->GetWorld(),FName("GameMap"));
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
