// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootGameGameMode.generated.h"

USTRUCT(BlueprintType)
struct FSpawnedBot
{
	GENERATED_BODY()


		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AShootGameCharacter> BotType;
};



UCLASS(minimalapi)
class AShootGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	bool bEverSetGameEnded;
public:
	AShootGameGameMode();


	virtual void BeginPlay() override;
protected: 


	/** match duration */
	UPROPERTY(EditAnywhere)
	int32 RoundTime;

	FTimerHandle TimerHandle_DefaultTimer;

	void DefaultTimer();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FSpawnedBot> SpawnBotInfo;
};



