// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyScoreItemData.h"
#include "MyEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API UMyEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	class UTextBlock* TB_PlayerId;
	class UEditableText* ET_PlayerNick;
	class UTextBlock* TB_PlayerScore;
	class UTextBlock* TB_KillCount;
	class UTextBlock* TB_KilledCount;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	int32 PlayerId=-1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	int32 CanEditNum;

	UFUNCTION(BlueprintCallable)
	void SetPlayerId(int32 x);

	UFUNCTION(BlueprintCallable)
	int32 GetPlayerId();

	UFUNCTION(BlueprintCallable)
	bool CanEditNick();

	UFUNCTION(BlueprintCallable)
	void SetMyPlayerName(FString x);


	UFUNCTION(BlueprintCallable)
	void ProcessScoreItemData(FMyScoreItemDataStruct sids);


	bool Initialize() override;
};
