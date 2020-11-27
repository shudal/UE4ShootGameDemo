// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShootGameCharacter.h"
#include "MyPlayerState.h"
#include "MyScoreItemData.h"
#include "MyGameState.h"
 
bool UMyEntryWidget::CanEditNick()
{
	// get my chracter  player id;
	int32 MyPlayerId = -1;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar && MyChar->IsLocallyControlled()) {
			AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (mps) { 
				MyPlayerId = mps->GetPlayerId();  
				break;
			}
		}
	}

	AMyGameState* gs = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));

	// 只有1个人可以改。
	int32 CanEditCount = CanEditNum;

	UE_LOG(LogClass, Log, TEXT("msavegame can edit name people count:%d"), CanEditCount);
	if (gs != nullptr) {
		TArray<UMyScoreItemData*> sids = gs->GetScoreItemArray();
		for (int32 i = 0; i < CanEditCount && i < sids.Num(); i++) { 
			UMyScoreItemData* sid = sids[i];

			// 当前widget是表示的这一个条目时， 且当前widget是表示的属于自己的条目时
			if (GetPlayerId() == sid->GetPlayerId() && GetPlayerId() == MyPlayerId) {
				return true;
			} 
		} 
	}
	return false; 
}

void UMyEntryWidget::SetPlayerId(int32 x) {
	PlayerId = x;
}

int32 UMyEntryWidget::GetPlayerId() {
	return PlayerId;
}

 
void UMyEntryWidget::SetMyPlayerName(FString x) {

	UE_LOG(LogClass, Log, TEXT("msavegame will set my player name to:%s"), *x);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar && MyChar->IsLocallyControlled()) {
			AMyPlayerState* mps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (mps) {
				mps->SetPlayerNick(x);
				UE_LOG(LogClass, Log, TEXT("msavegame seted my player name to:%s"), *x);
			}
		}
	}
}