// Fill out your copyright notice in the Description page of Project Settings.


#include "MyScoreItemData.h"
 
UMyScoreItemData::UMyScoreItemData(FString pn, float ps)
{
    SetPlayerName(pn);
    SetPlayerScore(ps); 
}

UMyScoreItemData::UMyScoreItemData(int32 pid, FString pn, float ps, int32 pr)
{
    SetPlayerId(pid);
    SetPlayerName(pn);
    SetPlayerScore(ps);
    SetPlayerRank(pr);
}

bool UMyScoreItemData::operator==(const UMyScoreItemData& rhs) const
{
    const UMyScoreItemData& lhs = *this;

    if (lhs.GetPlayerId() == rhs.GetPlayerId() && lhs.GetPlayerName() == rhs.GetPlayerName() && lhs.GetPlayerScore() == rhs.GetPlayerScore() && lhs.GetPlayerRank() == rhs.GetPlayerRank()) return true;
     
    return false;
}
 

UMyScoreItemData::UMyScoreItemData(const FObjectInitializer& oi) : Super(oi){  
}
/*

UMyScoreItemData::UMyScoreItemData(const UMyScoreItemData& other) {
    SetPlayerName(other.GetPlayerName());
    SetPlayerScore(other.GetPlayerScore());
    SetPlayerRank(other.GetPlayerRank());
}
*/
void UMyScoreItemData::SetPlayerName(FString pn)
{
    PlayerName = pn;
}

void UMyScoreItemData::SetPlayerScore(float ps)
{
    PlayerScore = ps;
}

FString UMyScoreItemData::GetPlayerName() const
{
    return PlayerName;
}

float UMyScoreItemData::GetPlayerScore() const
{
    return PlayerScore;
}


void UMyScoreItemData::SetPlayerRank(int32 pr) {
    PlayerRank = pr;
}
int32 UMyScoreItemData::GetPlayerRank() const{
    return PlayerRank;
}

 
void UMyScoreItemData::SetPlayerId(int32 x) {
    PlayerId = x;
}
 
int32 UMyScoreItemData::GetPlayerId() const {
    return PlayerId;
}
 

