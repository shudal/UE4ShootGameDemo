// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyScoreItemData.h"
#include "MySaveGame.generated.h" 
/**
 * 
 */
UCLASS()
class SHOOTGAME_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
protected:
      

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
        TArray<FMyScoreItemDataStruct> PlayerRank;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
        FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
        FString SaveSlotName;

    UPROPERTY(EditAnywhere)
        uint32 UserIndex;
     
public:
    void SetPlayerName(FString x); 
    void SetPlayerRank(TArray<UMyScoreItemData*> arr);

    void SetSaveSlotName(FString x);
    void SetUserIndex(int x);
     
    TArray<FMyScoreItemDataStruct> GetPlayerRank();
    FString GetSaveSlotName();
    uint32 GetUserIndex();
    FString GetPlayerName();
    UMySaveGame();
};
