// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "UserWidget_MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API UUserWidget_MainMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	FString SESSION_NAME = "SESSION_NAME";
protected:
	TSharedPtr<class FOnlineSessionSearch> SearchSettings;

	//FOnFindSessionsCompleteDelegate MyOnFindSessionsCompleteDelegate;
	//FOnCreateSessionCompleteDelegate MyOnCreateSessionCompleteDelegate;
	FString ToCreateRoomName;
	int32 ToJoinRoomIndex;
public:

	//FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	//FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
public: 
	UUserWidget_MainMenu(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void MyOnFindRoom();

	UFUNCTION(BlueprintCallable)
	void MyOnCreateRoom();


	UFUNCTION(BlueprintCallable)
		void MyOnJoinButtonClicked();


	UFUNCTION(BlueprintCallable)
		void SetToJoinRoomIndex(int32 x);


	UFUNCTION(BlueprintCallable)
	int32 GetToJoinRoomIndex();

	UFUNCTION(BlueprintCallable)
	void MyJoinSession(int32 SessionIndexInSearchResults);


	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);


	UFUNCTION(BlueprintCallable)
	void SetRoomName(FString x);

	UFUNCTION(BlueprintCallable)
	FString GetRoomName(); 
	 
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);


	UFUNCTION(BlueprintCallable)
		FString GetRoomInfoText();

};
