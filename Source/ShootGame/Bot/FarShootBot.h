// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BotCharacter.h"
#include "../Weapon.h"
#include "FarShootBot.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AFarShootBot : public ABotCharacter
{
	GENERATED_BODY()
	
private:
	bool bEverIniWeapon = false;
public: 
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	FWeaponData IniWeapon;


	FTimerHandle TimerHandle_DefaultTimer;

	virtual void DefaultTimer() override;
};
