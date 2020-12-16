// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_StickBot.h"
#include "../ShootGameCharacter.h"
#include "StickBot.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAIC_StickBot::DefaultTimer() { 
	UE_LOG(LogClass, Log, TEXT("myaic timer tick"));

	AStickBot* myc = Cast<AStickBot>(GetPawn());
	if (myc != nullptr) { 
		if (myc->GetSkillName() != "None") {
			myc->SetSkillName("None");
		}

		if (myc->IsBeingFuck()) {
			if (bIsBeingFuck == false) {
				GetBlackboardComponent()->SetValueAsBool(BB_KEY_IsBeingFuck, true);
				bIsBeingFuck = true;
			}
			UE_LOG(LogClass, Log, TEXT("mbot char is being fuck"));
		}
		else { 
			if (bIsBeingFuck == true) { 
				GetBlackboardComponent()->SetValueAsBool(BB_KEY_IsBeingFuck, false);
				bIsBeingFuck = false;
			}
			UE_LOG(LogClass, Log, TEXT("mbot char is not being fuck"));
		}

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