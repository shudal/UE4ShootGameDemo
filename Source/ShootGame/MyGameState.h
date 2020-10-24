// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
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
public:
	AMyGameState();

	void BeginPlay() override;
	void SetRemainingTime(int32 NewRemainingTime);

	UFUNCTION(BlueprintPure, Category = "ShootGame")
		FString GetRemaingTimeText();

	uint32 GetRemainingTime();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION(BlueprintPure, Category = "ShootGame")
		FString GetScoreListText();
};
