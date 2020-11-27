// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"


AMyPlayerState::AMyPlayerState() {
	PlayerScore = 0;
	PlayerNick = TEXT("NoneName");
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

	DOREPLIFETIME(AMyPlayerState, PlayerNick); 
}

TArray<FWeaponData>& AMyPlayerState::GetWeaponData()
{
	// TODO: insert return statement here
	return WeaponData;
}

FString AMyPlayerState::GetPlayerNick() { 
	return PlayerNick;
}

void AMyPlayerState::SetPlayerNick_Implementation(const FString &x) {
	PlayerNick = x;
}

int32 AMyPlayerState::GetNowWeaponIndex() {
	return NowWeaponIndex;
}

 
void AMyPlayerState::SetNowWeaponIndex(int32 x) {
	NowWeaponIndex = x;
}