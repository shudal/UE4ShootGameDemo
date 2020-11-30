// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "ShootGameCharacter.h"


AMyPlayerState::AMyPlayerState() {
	PlayerScore = 0;
	PlayerNick = TEXT("NoneName");

	NowWeaponIndex = 0;
	CharLifeType = ECharLifeType::CLT_ALIVE;
	SetBlood(MAX_BLOOD);
	KillCount = 0;
	KilledCount = 0;
}
float AMyPlayerState::GetBlood()
{
	return Blood;
}
void AMyPlayerState::SetBlood_Implementation(float x)
{

	UE_LOG(LogClass, Log, TEXT("mmul set blood called"));
	if (CharLifeType == ECharLifeType::CLT_ALIVE) { 
		if (x <= MAX_BLOOD) {
			Blood = x; 
			if (x <= 0) {
				SetLifeState(ECharLifeType::CLT_DEAD);
			}
		}
	}
	else if (CharLifeType == ECharLifeType::CLT_DEAD) {

	}
}
AShootGameCharacter* AMyPlayerState::GetMyChar()
{
	return MyChar;
}
void AMyPlayerState::SetMyChar(AShootGameCharacter* x)
{
	MyChar = x;
}
ECharLifeType AMyPlayerState::GetLifeState()
{
	return CharLifeType;
}
void AMyPlayerState::SetLifeState_Implementation(ECharLifeType x)
{
	CharLifeType = x;
	if (x == ECharLifeType::CLT_ALIVE) {
		Blood = MAX_BLOOD;
	}
	else if (x == ECharLifeType::CLT_DEAD) {
		if (MyChar != nullptr) {
			//MyChar->GetMesh()->SetSimulatePhysics(true);
			//MyChar->SetSkillName("Skill3");
			UpdateKilledCount(1); 
		}
		else {
			FString msg = FString::Printf(TEXT("char dead,but mychar pointer is null"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, msg);

		}
	}
}
int32 AMyPlayerState::GetKillCount()
{
	return KillCount;
}
void AMyPlayerState::UpdateKillCount_Implementation(int32 x)
{
	KillCount += x;
}
int32 AMyPlayerState::GetKilledCount()
{
	return KilledCount;
}
void AMyPlayerState::UpdateKilledCount_Implementation(int32 x)
{
	KilledCount += x;
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
	
	DOREPLIFETIME(AMyPlayerState, WeaponData);

	DOREPLIFETIME(AMyPlayerState, NowWeaponIndex);

	DOREPLIFETIME(AMyPlayerState, CharLifeType);

	DOREPLIFETIME(AMyPlayerState, Blood);

	DOREPLIFETIME(AMyPlayerState, KillCount);

	DOREPLIFETIME(AMyPlayerState, KilledCount);
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