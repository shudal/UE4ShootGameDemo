// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShootGameCharacter.h"
#include "MyPlayerState.h"

void AMyGameState::SetRemainingTime(int32 NewRemainingTime)
{
	RemainingTime = NewRemainingTime;
}

uint32 AMyGameState::GetRemainingTime()
{
	return RemainingTime;
}

void  AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, RemainingTime);

}

FString AMyGameState::GetRemaingTimeText() {
	FString ans;
	if (RemainingTime > 0) { 
		ans = FString::Printf(TEXT("Time Left: %d second(s)"), RemainingTime);
	}
	else {
		FString WinPlayerName = "None";
		float WinPlayerScore = -1;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
		for (auto Actor : FoundActors) {
			AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
			if (MyChar) {
				AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
				if (mps) {
					float  s = mps->GetPlayerScore();
					if (s > WinPlayerScore) {
						WinPlayerScore = s;
						WinPlayerName = MyChar->GetName();
					}
				}
			}
		}
		 
		ans = FString::Printf(TEXT("Winner Player Local Name: %s,\nWinner Score: %f\n"), *WinPlayerName, WinPlayerScore);
	}
	return ans;
}
AMyGameState::AMyGameState() { 
}
void AMyGameState::BeginPlay() {
	Super::BeginPlay(); 
}
FString AMyGameState::GetScoreListText() { 

	FString ans = TEXT("");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);

	struct S1 {
		float score;
		FString txt;
		S1(float s, FString t) {
			score = s;
			txt = t;
		}
	};
	TArray<S1> arrs;

	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar) { 
			AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (mps) {
				float  sco = mps->GetPlayerScore();
				FString s = FString::Printf(TEXT("Local Player Name: %s, Score: %f\n"), *MyChar->GetName(), sco);
				arrs.Add(S1(sco, s));
			} 
		}
	}
	arrs.Sort([](const S1& s1, const S1& s2){
		return s1.score > s2.score;
	});

	for (auto tmps : arrs) {
		ans = ans + tmps.txt;
	}
	 
	return ans;
}