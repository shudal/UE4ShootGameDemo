// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyScoreItemData.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EScoreItemType : uint8 {
	SIT_Normal,
	SIT_Head
};

USTRUCT(BlueprintType)
struct FMyScoreItemDataStruct
{
	GENERATED_BODY()

		EScoreItemType ScoreItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerRank; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerScore;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 KillCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 KilledCount;

	FMyScoreItemDataStruct() {
		ScoreItemType = EScoreItemType::SIT_Normal;

		PlayerId = -1;
		PlayerName = "NoneName";
		PlayerScore = 0;
		PlayerRank = -1;
		KillCount = 0;
		KilledCount = 0;
	}
	FMyScoreItemDataStruct(int32 pid, FString pn, float ps, int32 pr) {
		PlayerId = pid;
		PlayerName = pn;
		PlayerScore = ps; 
		PlayerRank = pr;
	}

	bool operator==(const FMyScoreItemDataStruct& rhs) const {
		if (PlayerId == rhs.PlayerId && PlayerName == rhs.PlayerName && PlayerScore == rhs.PlayerScore && PlayerRank == rhs.PlayerRank) {
			return true;
		}
		else {
			return false;
		}
	}
};

UCLASS(BlueprintType)
class SHOOTGAME_API UMyScoreItemData : public UObject
{
	GENERATED_BODY()
protected:



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayerScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerRank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerId;
public: 
	UMyScoreItemData() = default;
	UMyScoreItemData(const FObjectInitializer& oi);
	UMyScoreItemData(FString pn, float ps); 
	UMyScoreItemData(int32 pid, FString pn, float ps, int32 pr);
	bool operator==(const UMyScoreItemData& rhs) const;
	 

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString pn);

	UFUNCTION(BlueprintCallable)
	void SetPlayerScore(float ps);


	UFUNCTION(BlueprintCallable)
		void SetPlayerRank(int32 pr);

	UFUNCTION(BlueprintCallable)
	FString GetPlayerName() const;

	UFUNCTION(BlueprintPure)
	float GetPlayerScore() const;

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerRank() const;


	UFUNCTION(BlueprintCallable)
		void SetPlayerId(int32 x);

	UFUNCTION(BlueprintCallable)
		int32 GetPlayerId() const;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMyScoreItemDataStruct ItemData;
};
