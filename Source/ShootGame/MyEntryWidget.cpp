// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEntryWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShootGameCharacter.h"
#include "MyPlayerState.h"
#include "MyScoreItemData.h"
#include "MyGameState.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
 
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

void UMyEntryWidget::ProcessScoreItemData(FMyScoreItemDataStruct sids)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	int32 MyPlayerId=-1;
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

	if (sids.ScoreItemType == EScoreItemType::SIT_Normal) {
		if (TB_PlayerId != nullptr) TB_PlayerId->SetText(FText::FromString(FString::FromInt(sids.PlayerId)));
		if (ET_PlayerNick != nullptr) ET_PlayerNick->SetText(FText::FromString(sids.PlayerName));
		if (TB_PlayerScore != nullptr) TB_PlayerScore->SetText(FText::FromString(FString::FromInt(sids.PlayerScore)));
		if (TB_KillCount != nullptr) TB_KillCount->SetText(FText::FromString(FString::FromInt(sids.KillCount)));
		if (TB_KilledCount != nullptr) TB_KilledCount->SetText(FText::FromString(FString::FromInt(sids.KilledCount)));

		if (ET_PlayerNick != nullptr) {
			if (sids.PlayerRank <= CanEditNum) { 
				if (sids.PlayerId == MyPlayerId) {
					ET_PlayerNick->SetIsReadOnly(false);
				}
				else {
					ET_PlayerNick->SetIsReadOnly(true);
				}
			}
			else { 
				ET_PlayerNick->SetIsReadOnly(true);
			}
		}

	}
	else { 
		if (TB_PlayerId != nullptr) TB_PlayerId->SetText(FText::FromString("Id"));
		if (ET_PlayerNick != nullptr) {
			ET_PlayerNick->SetText(FText::FromString("Nickname"));
			ET_PlayerNick->SetIsReadOnly(true);
		}
		if (TB_PlayerScore != nullptr) TB_PlayerScore->SetText(FText::FromString("Score"));
		if (TB_KillCount != nullptr) TB_KillCount->SetText(FText::FromString("Kill"));
		if (TB_KilledCount != nullptr) TB_KilledCount->SetText(FText::FromString("Killed"));
 
	}
}

bool UMyEntryWidget::Initialize() { 
	if (!Super::Initialize()) {
		return false;
	}

	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("TB_PlayerId"))) {
		TB_PlayerId = utb; 
	}
	if (UEditableText* uet = Cast<UEditableText>(GetWidgetFromName("ET_PlayerNick"))) {
		ET_PlayerNick = uet;
	}
	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("TB_PlayerScore"))) {
		TB_PlayerScore = utb;
	}
	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("TB_KillCount"))) {
		TB_KillCount = utb;
	}
	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("TB_KilledCount"))) {
		TB_KilledCount = utb;
	}
	return true;
}