// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCompoundWidget.h"
#include "SlAiTypes.h"
#include "SlAiGameOptionWidget.h"
#include "SlAiMenuItemWidget.h"

struct MenuGroup;
class SlAiChooseRecordWidget;
class SlAiNewGameWidget;

/**
 * 
 */
class SLAICOURSE_API SSlAiMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSlAiMenuWidget)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	//重写Tick
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:

	//绑定到各个按钮点击
	void MenuItemOnClicked(EMenuItem::Type ItemType);

	//音量变化
	void OnChangeCulture(ECultureTeam NewCulture);
	
	//音量变化
	void OnChangeVolume(float MusicVolume,float SoundVolume);

	//初始化所有的控件
	void InitMenuList();

	//选择显示的界面
	void ChooseWidget(EMenuType::Type WidgetType);

	//修改菜单大小
	void ResetWidgetSize(float NewWidth, float NewHeight);

	//初始化动画
	void InitAnimation();

	//关闭播放动画
	void PlayCloseAnim(EMenuType::Type NewMenu);

	//退出游戏
	void QuitGame();

	//进入游戏
	void EnterGame();
private:
		
	//获取Menu样式
	const struct FSlAiMenuStyle *MenuStyle;
	
	//根节点盒子
	TSharedPtr<SBox> RootSizeBox;

	//标题
	TSharedPtr<STextBlock> TitleText;

	//垂直列表
	TSharedPtr<SVerticalBox> ContentBox;

	//保存菜单组
	TMap<EMenuType::Type, TSharedPtr<MenuGroup>> MenuMap;

	TSharedPtr<SlAiGameOptionWidget> GameOptionWidget;
	TSharedPtr<SlAiNewGameWidget> NewGameWidget;
	TSharedPtr<SlAiChooseRecordWidget> ChooseRecordWidget;

	//动画
	FCurveSequence MenuAnimation;
	//曲线控制器
	FCurveHandle MenuCurve;
	//当前高度
	float CurrHeight;
	//是否已经显示
	bool IsMenuShow;
	//是否已经锁住按钮
	bool ControlLocked;
	//动画状态
	EMenuAnim::Type AnimState;

	//当前菜单
	EMenuType::Type CurrMenu;
};
