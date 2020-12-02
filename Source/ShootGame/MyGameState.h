// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h" 
#include "MyScoreItemData.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AMyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected: 
	UPROPERTY(EditAnywhere, Replicated)
	int32 RemainingTime;

	TArray<class AShootGameCharacter*> MyChars;

	TArray<UMyScoreItemData*> PlayerRank;


	UPROPERTY(EditAnywhere, Replicated)
	int32 MyGameStatus = 0;

	 

	int32 GAME_STATUS_STARTED = 1;
	int32 GAME_STATUS_ENDED = 2;
public:
	AMyGameState();

	void BeginPlay() override;
	void SetRemainingTime(int32 NewRemainingTime);
	void SetGameStatus(int32 x);

	UFUNCTION(BlueprintPure, Category = "ShootGame")
		FString GetRemaingTimeText();

	uint32 GetRemainingTime();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(BlueprintPure, Category = "ShootGame")
		FString GetScoreListText();

	 
	TArray<UMyScoreItemData*> GetScoreItemArray();
	TArray<FMyScoreItemDataStruct> GetScoreItems();

	bool IsGameFinished();


	FTimerHandle TimerHandle_DefaultTimer;

	void DefaultTimer();
};
