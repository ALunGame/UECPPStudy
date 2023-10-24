// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "SlAiGameWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct SLAICOURSE_API FSlAiGameStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSlAiGameStyle();
	virtual ~FSlAiGameStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSlAiGameStyle& GetDefault();

	//普通
	UPROPERTY(EditAnywhere, Category = Package)
	FSlateBrush NormalContainerBrush;

	//选中
	UPROPERTY(EditAnywhere, Category = Package)
	FSlateBrush ChooseContainerBrush;

	//没有物品
	UPROPERTY(EditAnywhere, Category = Package)
	FSlateBrush EmptyBrush;
	
	/*
	* 60号字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_60;

	/*
	* 50号字体带描边
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_Outline_50;

	/*
	* 40号字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_40;

	/*
	* 40号字体带描边
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_Outline_40;

	/*
	* 30号字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_30;

	/*
	* 30号字体带描边
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_Outline_30;

	/*
	* 20号字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_20;

	/*
	* 20号字体带描边
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_Outline_20;

	/*
	* 16号字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_16;
	
	/*
	* 16号字体带描边
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FSlateFontInfo Font_Outline_16;
	
	/*
	* 白色字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FLinearColor FontColor_White;

	/*
	* 黑色字体
	*/
	UPROPERTY(EditAnywhere, Category = Common)
	FLinearColor FontColor_Black;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USlAiGameWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSlAiGameStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
