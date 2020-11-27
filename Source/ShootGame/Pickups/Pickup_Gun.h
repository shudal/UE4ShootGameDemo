// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"  
#include "../Weapon.h"
#include "Pickup_Gun.generated.h"
 

/**
 * 
 */
UCLASS()
class SHOOTGAME_API APickup_Gun : public APickup
{
	GENERATED_BODY()
	
protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	class UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	FWeaponData MyWeaponData;

	void BeginPlay() override; 


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame") 
		USoundBase* BePickedSound;
public:
	APickup_Gun();


	UFUNCTION(BlueprintCallable)
	FWeaponData GetWeaponData();


	UFUNCTION(BlueprintCallable)
	void BePicked();
};
