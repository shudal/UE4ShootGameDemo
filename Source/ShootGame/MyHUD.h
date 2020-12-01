// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY()
	class AMyPlayerState* mps;
	class AMyGameState* MyGameState;

protected:
	void SetMyPlayerState();
	 
	class AShootGameCharacter* mchar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	class UListView* MListView;

	class UTextBlock* TB_RelifeTIp;
	class UTextBlock* TB_PlayerStateList;

public: 
	UMyHUD(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FString GetScoreText();


	UFUNCTION(BlueprintCallable)
	float GetMyScore();

	UFUNCTION(BlueprintCallable)
		FString GetPersonalInfoText();

	UFUNCTION(BlueprintCallable)
		void MyPlayerShoot();


	UFUNCTION(BlueprintCallable)
		void SetScoreList();

	bool Initialize() override;


	FTimerHandle TimerHandle_DefaultTimer;

	void DefaultTimer();
};
