// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIC_StickBot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AAIC_StickBot : public AAIController
{
	GENERATED_BODY()
	
private:
	bool bIsBeingFuck = false;
protected:

	UFUNCTION(BlueprintCallable)
		void SetMyTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BB_KEY_IsBeingFuck;

public: 
	FTimerHandle TimerHandle_DefaultTimer; 
	void DefaultTimer();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> SkillNames;

	UFUNCTION(BlueprintCallable)
	void AttackRandom();
};
