// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShootGameCharacter.h"
#include "MyPlayerState.h"
#include "MyScoreItemData.h"

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
	DOREPLIFETIME(AMyGameState, MyGameStatus); 

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
	 
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AMyGameState::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);

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
				auto playerNick = mps->GetPlayerNick();
				FString s = FString::Printf(TEXT("Player Nick: %s, KillCount:%d, KilledCount:%d,Blood: %f, Score: %f\n"), *playerNick, mps->GetKillCount(), mps->GetKilledCount(),mps->GetBlood(),sco);
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

TArray<FMyScoreItemDataStruct> AMyGameState::GetScoreItems() {
	TArray<FMyScoreItemDataStruct> ans;


	FMyScoreItemDataStruct head;
	head.PlayerScore = INT32_MAX;
	head.ScoreItemType = EScoreItemType::SIT_Head;
	ans.Add(head);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar) {
			AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (mps) {
				FMyScoreItemDataStruct ids;
				ids.PlayerId = mps->GetPlayerId();
				ids.PlayerName = mps->GetPlayerNick();
				ids.PlayerScore = mps->GetPlayerScore();
				ids.KillCount = mps->GetKillCount();
				ids.KilledCount = mps->GetKilledCount();
				ans.AddUnique(ids);
			}
		}
	}
	ans.Sort([](const FMyScoreItemDataStruct& s1, const FMyScoreItemDataStruct& s2) {
		return s1.PlayerScore > s2.PlayerScore;
	});
	for (int32 i = 0; i < PlayerRank.Num(); i++) {
		ans[i].PlayerRank = i + 1;
	}
	return ans;
}
TArray<UMyScoreItemData*> AMyGameState::GetScoreItemArray() { 
	PlayerRank.Empty();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	  

	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar) {
			AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (mps) {  
				UMyScoreItemData* msid = NewObject<UMyScoreItemData>(this, UMyScoreItemData::StaticClass());
				msid->SetPlayerName(mps->GetPlayerNick());
				msid->SetPlayerScore(mps->GetPlayerScore());
				msid->SetPlayerId(mps->GetPlayerId()); 
				PlayerRank.AddUnique(msid);
			}
		}
	}
	PlayerRank.Sort([](const UMyScoreItemData & s1,const UMyScoreItemData & s2) {
		return s1.GetPlayerScore() > s2.GetPlayerScore();
	});
	for (int32 i = 0; i < PlayerRank.Num(); i++) {
		PlayerRank[i]->SetPlayerRank(i);
	}
	return PlayerRank;
}


bool AMyGameState::IsGameFinished() {
	if (MyGameStatus == GAME_STATUS_ENDED) {
		return true;
	}
	else {
		return false;
	}
}


void AMyGameState::SetGameStatus(int32 x) {
	MyGameStatus = x;
}


void AMyGameState::DefaultTimer() { 
}