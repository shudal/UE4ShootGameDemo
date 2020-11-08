// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, Replicated)
	float PlayerScore;


	UPROPERTY(EditAnywhere, Replicated)
	FString PlayerNick;
	 
public:
	AMyPlayerState();
	 

	UFUNCTION(BlueprintPure, Category = "ShootGame")
	float GetPlayerScore();


	UFUNCTION(BlueprintPure, Category = "ShootGame")
	FString GetPlayerNick();


	UFUNCTION(Server, Reliable)
		void SetPlayerNick(const FString &x);
	void SetPlayerNick_Implementation(const FString &x); 

	void UpdateScore(float);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	 
};
