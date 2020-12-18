// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ShootGameCharacter.h"
#include "BotCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API ABotCharacter : public AShootGameCharacter
{
	GENERATED_BODY()
	

private:
	int32 TimerTickCount;
protected:
	bool bIsBeingFuck = false;

	// 伤害持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		int32 FuckLastTime;

public:
	ABotCharacter();

	virtual void UpdateBlood(float x) override;

	UFUNCTION(BlueprintCallable)
		void SetIsBeingFuck(bool x);


	UFUNCTION(BlueprintCallable)
		bool IsBeingFuck();



	FTimerHandle TimerHandle_DefaultTimer;
	virtual void DefaultTimer();


	virtual void BeginPlay() override;


};
