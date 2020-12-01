// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AMyGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> MyWidgetClass;

	//class UUserWidget* MyWidget;
	class UMyHUD* MyGameUW;
public:
	 
	virtual void BeginPlay() override; 
};
