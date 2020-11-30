// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Weapon.h"
#include "MyPlayerState.generated.h"

UENUM(BlueprintType)
enum class ECharLifeType : uint8 {
	CLT_ALIVE,
	CLT_DEAD
};

/**
 * 
 */
UCLASS()
class SHOOTGAME_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
private:
	float MAX_BLOOD = 100;
protected:

	UPROPERTY(EditAnywhere, Replicated)
	float PlayerScore;


	UPROPERTY(EditAnywhere, Replicated)
	FString PlayerNick;
	 

	UPROPERTY(EditAnywhere, Replicated)
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditAnywhere, Replicated)
	int32 NowWeaponIndex;

	UPROPERTY(EditAnywhere, Replicated)
	ECharLifeType CharLifeType;


	UPROPERTY(EditAnywhere, Replicated)
	float Blood;


	UPROPERTY(EditAnywhere, Replicated)
	int32 KillCount;

	UPROPERTY(EditAnywhere, Replicated)
	int32 KilledCount;

	class AShootGameCharacter* MyChar;
public:
	AMyPlayerState();
	 

	UFUNCTION(BlueprintCallable)
	float GetBlood();

	UFUNCTION(Server, Reliable)
	void SetBlood(float x); 
	void SetBlood_Implementation(float x);
	

	class AShootGameCharacter* GetMyChar();
	void SetMyChar(class AShootGameCharacter* x);

	ECharLifeType GetLifeState();

	UFUNCTION(Server, Reliable)
		void SetLifeState(ECharLifeType x);
	void SetLifeState_Implementation(ECharLifeType x);
	

	UFUNCTION(BlueprintCallable)
		int32 GetKillCount(); 
	UFUNCTION(Server, Reliable)
		void UpdateKillCount(int32 x);
	void UpdateKillCount_Implementation(int32 x);

	UFUNCTION(BlueprintCallable)
		int32 GetKilledCount();
	UFUNCTION(Server, Reliable)
		void UpdateKilledCount(int32 x);
	void UpdateKilledCount_Implementation(int32 x);

	UFUNCTION(BlueprintPure, Category = "ShootGame")
	float GetPlayerScore();


	UFUNCTION(BlueprintPure, Category = "ShootGame")
	FString GetPlayerNick();


	UFUNCTION(Server, Reliable)
		void SetPlayerNick(const FString &x);
	void SetPlayerNick_Implementation(const FString &x); 

	void UpdateScore(float);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	 
	TArray<FWeaponData>& GetWeaponData();


	UFUNCTION(BlueprintCallable)
		int32 GetNowWeaponIndex();


	UFUNCTION(BlueprintCallable)
		void SetNowWeaponIndex(int32 x);


};
