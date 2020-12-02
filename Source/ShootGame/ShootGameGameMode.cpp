// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootGameGameMode.h"
#include "ShootGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MyGameState.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"


AShootGameGameMode::AShootGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	}

	RoundTime = 15;
	bEverSetGameEnded = false;
}

void AShootGameGameMode::BeginPlay() { 

	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState) { 
		MyGameState->SetRemainingTime(RoundTime);
		MyGameState->SetGameStatus(1);
	}


	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AShootGameGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);


	bEverSetGameEnded = false;
}

void AShootGameGameMode::DefaultTimer()
{
	 
	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState && MyGameState->GetRemainingTime() > 0) {
		MyGameState->SetRemainingTime(MyGameState->GetRemainingTime() - 1);
		UE_LOG(LogClass, Log, TEXT("Game Remaing Time: %d"), MyGameState->GetRemainingTime());
	}
	else { 
		if (bEverSetGameEnded == false) { 
			bEverSetGameEnded = true;
			MyGameState->SetGameStatus(2);
		}
		
	}
}
