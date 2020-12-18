// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Bot.h"

#include "../ShootGameCharacter.h"
#include "BotCharacter.h"
#include "StickBot.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAIC_Bot::AAIC_Bot() {
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}
void AAIC_Bot::DefaultTimer() {
	UE_LOG(LogClass, Log, TEXT("myaic timer tick"));

	ABotCharacter* myc = Cast<ABotCharacter>(GetPawn());
	if (myc != nullptr) {  
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


void AAIC_Bot::SetMyTimer() {
	if (GetWorld()) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AAIC_Bot::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}
}

void AAIC_Bot::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_Bot::OnTargetPerceptionUpdated);
}

void AAIC_Bot::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
}
