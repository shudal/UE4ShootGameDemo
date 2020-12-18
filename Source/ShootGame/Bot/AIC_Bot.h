// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIC_Bot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AAIC_Bot : public AAIController
{
	GENERATED_BODY()
private:
	bool bIsBeingFuck = false;
protected:

	UFUNCTION(BlueprintCallable)
		void SetMyTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName BB_KEY_IsBeingFuck;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		class UAIPerceptionComponent* AIPerceptionComponent;
public:
	AAIC_Bot();

	FTimerHandle TimerHandle_DefaultTimer;
	void DefaultTimer();


	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
