// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API UMyEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	

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
};
