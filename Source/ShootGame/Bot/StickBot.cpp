// Fill out your copyright notice in the Description page of Project Settings.


#include "StickBot.h"


AStickBot::AStickBot() {
	FuckLastTime = 5;

	TimerTickCount = 0;
}
void  AStickBot::UpdateBlood(float x) {
	Super::UpdateBlood(x);


	UE_LOG(LogClass, Log, TEXT("mbot bot update blood"));
	if (x < 0) {
		SetIsBeingFuck(true);
	}
}

void AStickBot::SetIsBeingFuck(bool x) {
	bIsBeingFuck = x;
}
bool AStickBot::IsBeingFuck() {
	return bIsBeingFuck;
}

void AStickBot::DefaultTimer() { 
	UE_LOG(LogClass, Log, TEXT("mbot timer tick"));
	 
	TimerTickCount++;


	if(GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer cout:%d"), TimerTickCount);
	if (TimerTickCount % FuckLastTime == 0) {
		TimerTickCount = 0;
		if (GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer set count =0"));
		if (IsBeingFuck()) {

			if (GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer setnot fuck"));
			SetIsBeingFuck(false);
		}
	}
}


void AStickBot::BeginPlay() {
	Super::BeginPlay();

	if (GetWorld() != nullptr) { 
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AStickBot::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}
}