// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootGameGameMode.h"
#include "ShootGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MyGameState.h"

AShootGameGameMode::AShootGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	
	}

	RoundTime = 300;
}

void AShootGameGameMode::BeginPlay() { 

	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState) { 
		MyGameState->SetRemainingTime(RoundTime);
	}


	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AShootGameGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

}

void AShootGameGameMode::DefaultTimer()
{
	AMyGameState* const MyGameState = Cast<AMyGameState>(GameState);
	if (MyGameState && MyGameState->GetRemainingTime() > 0) {
		MyGameState->SetRemainingTime(MyGameState->GetRemainingTime() - 1);
		UE_LOG(LogClass, Log, TEXT("Game Remaing Time: %d"), MyGameState->GetRemainingTime());
	}
}
