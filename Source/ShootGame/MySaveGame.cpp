// Fill out your copyright notice in the Description page of Project Settings.


#include "MySaveGame.h"
#include "MyScoreItemData.h"

void UMySaveGame::SetPlayerName(FString x)
{
    PlayerName = x;
}

/*

void UMySaveGame::SetPlayerRankOfClass(TArray<UMyScoreItemData*> arr)
{
    for (UMyScoreItemData* x : arr) {
        UMyScoreItemData y(x->GetPlayerName(), x->GetPlayerScore());
        y.SetPlayerId(x->GetPlayerId());
        y.SetPlayerRank(x->GetPlayerRank());

        PlayerRankOfClass.Add(y);
    }
}
*/

void UMySaveGame::SetPlayerRank(TArray<UMyScoreItemData*> arr)
{
    for (UMyScoreItemData* x : arr) {
        /*
        UMyScoreItemData y(x->GetPlayerName(), x->GetPlayerScore());
        y.SetPlayerId(x->GetPlayerId());
        y.SetPlayerRank(x->GetPlayerRank());
        
        */
        FMyScoreItemDataStruct sid(x->GetPlayerId(), x->GetPlayerName(), x->GetPlayerScore(), x->GetPlayerRank());
        PlayerRank.AddUnique(sid);
    }
}
 
UMySaveGame::UMySaveGame() {
    SaveSlotName = TEXT("MySaveGame"); 
    UserIndex = 0; 
    PlayerName = TEXT("NoneName");
}

void UMySaveGame::SetSaveSlotName(FString x) {
    SaveSlotName = x;
}
void UMySaveGame::SetUserIndex(int x) {
    UserIndex = x;
}
FString UMySaveGame::GetSaveSlotName() {
    return SaveSlotName;
}
uint32 UMySaveGame::GetUserIndex() {
    return UserIndex;
}

TArray<FMyScoreItemDataStruct> UMySaveGame::GetPlayerRank() {
    return PlayerRank;
}

FString UMySaveGame::GetPlayerName() {
    return PlayerName;
}