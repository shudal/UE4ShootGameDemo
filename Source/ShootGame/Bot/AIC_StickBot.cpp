// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_StickBot.h"
#include "../ShootGameCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void AAIC_StickBot::DefaultTimer() { 
	UE_LOG(LogClass, Log, TEXT("myaic timer tick"));

	AShootGameCharacter* myc = Cast<AShootGameCharacter>(GetPawn());
	if (myc != nullptr && myc->GetSkillName() != "None") {
		myc->SetSkillName("None");
	}
}


void AAIC_StickBot::SetMyTimer() {
	if (GetWorld()) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AAIC_StickBot::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}
}

void AAIC_StickBot::AttackRandom() {
	if (SkillNames.Num() > 0) { 
		auto i = UKismetMathLibrary::RandomIntegerInRange(0, SkillNames.Num() - 1);
		AShootGameCharacter* myc = Cast<AShootGameCharacter>(GetPawn());
		if (myc != nullptr) {
			myc->SetSkillName(SkillNames[i]);
		}
	}
}