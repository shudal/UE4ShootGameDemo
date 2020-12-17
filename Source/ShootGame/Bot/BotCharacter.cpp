// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"


ABotCharacter::ABotCharacter() {
	FuckLastTime = 15;

	TimerTickCount = 0;
}
void  ABotCharacter::UpdateBlood(float x) {
	Super::UpdateBlood(x);


	UE_LOG(LogClass, Log, TEXT("mbot bot update blood"));
	if (x < 0) {
		SetIsBeingFuck(true);
	}
}

void ABotCharacter::SetIsBeingFuck(bool x) {
	bIsBeingFuck = x;
	if (x) {
		TimerTickCount = 0;
	}
}
bool ABotCharacter::IsBeingFuck() {
	return bIsBeingFuck;
}

void ABotCharacter::DefaultTimer() {
	UE_LOG(LogClass, Log, TEXT("mbot timer tick"));

	TimerTickCount++;


	if (GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer cout:%d"), TimerTickCount);
	if (TimerTickCount % FuckLastTime == 0) {
		TimerTickCount = 0;
		if (GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer set count =0"));
		if (IsBeingFuck()) {

			if (GetLocalRole() == ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot timer setnot fuck"));
			SetIsBeingFuck(false);
		}
	}
}


void ABotCharacter::BeginPlay() {
	Super::BeginPlay();

	if (GetWorld() != nullptr) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ABotCharacter::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}
}