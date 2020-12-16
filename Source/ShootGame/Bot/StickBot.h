// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ShootGameCharacter.h"
#include "StickBot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AStickBot : public AShootGameCharacter
{
	GENERATED_BODY()
	
private:
	int32 TimerTickCount;
protected:
	bool bIsBeingFuck = false;

	// �˺�����ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	int32 FuckLastTime;

public:
	AStickBot();

	void UpdateBlood(float x) override;

	UFUNCTION(BlueprintCallable)
	void SetIsBeingFuck(bool x);


	UFUNCTION(BlueprintCallable)
	bool IsBeingFuck();



	FTimerHandle TimerHandle_DefaultTimer; 
	void DefaultTimer();


	virtual void BeginPlay() override;
};
