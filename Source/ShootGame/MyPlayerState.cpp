// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"


AMyPlayerState::AMyPlayerState() {
	PlayerScore = 0;
}
float AMyPlayerState::GetPlayerScore()
{
	return PlayerScore;
}

void AMyPlayerState::UpdateScore(float x)
{ 
	PlayerScore += x;
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, PlayerScore);
}
