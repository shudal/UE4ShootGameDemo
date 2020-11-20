// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_MainMenu.h"

#include "Kismet/GameplayStatics.h" 
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemSessionSettings.h"
#include "OnlineSubsystemUtils.h"  
 

UUserWidget_MainMenu::UUserWidget_MainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
 
	ToCreateRoomName = "Hi";
	//+FString::Printf(TEXT("%f"), UGameplayStatics::GetTimeSeconds(GetWorld()));
	
}

void UUserWidget_MainMenu::SetRoomName(FString x) {
	ToCreateRoomName = x;
}
 
FString UUserWidget_MainMenu::GetRoomName() {
	return ToCreateRoomName;
}
void UUserWidget_MainMenu::MyJoinSession(int32 SessionIndexInSearchResults)
{
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		TArray<ULocalPlayer*> lps = UGameplayStatics::GetGameInstance(GetWorld())->GetLocalPlayers();
		TSharedPtr<const FUniqueNetId> UserId;
		if (lps.Num() > 0) {
			 UserId = lps[0]->GetPreferredUniqueNetId().GetUniqueNetId();
		}

		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UUserWidget_MainMenu::OnJoinSessionComplete));
			
			if (SearchSettings != nullptr) { 
				if (SessionIndexInSearchResults >= 0 && SessionIndexInSearchResults < SearchSettings->SearchResults.Num()) {
					auto tmpx = SearchSettings->SearchResults[SessionIndexInSearchResults].Session.SessionSettings.Settings.FindRef(FName(SESSION_NAME));
					auto tmpy = tmpx.Data;
					auto tmpz = tmpy.ToString();
					if (!tmpz.IsEmpty()) {
						UE_LOG(LogClass, Log, TEXT("mmainmenu room name is :%s"), *tmpz);
						Sessions->JoinSession(*UserId, FName(tmpz), SearchSettings->SearchResults[SessionIndexInSearchResults]);
					}
					else {
						UE_LOG(LogClass, Log, TEXT("mmainmenu room name string is empty"));
					}
				}
				else {
					UE_LOG(LogClass, Log, TEXT("mroom session index in searchresult not valid"));

				}
			} else{
				UE_LOG(LogClass, Log, TEXT("mroom searchsettings is null ptr"));
			}
		}
	}
}
void UUserWidget_MainMenu::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogClass, Log, TEXT("OnJoinSessionComplete %s bSuccess: %d"), *InSessionName.ToString(), static_cast<int32>(Result));

	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			// travel
			FString URL; 
			if (Sessions->GetResolvedConnectString(InSessionName, URL))
			{
				UE_LOG(LogOnlineGame, Warning, TEXT("mroom resolve url ok,url:%s"), *URL);
				APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PC)
				{ 
					UE_LOG(LogOnlineGame, Warning, TEXT("mroom pc is valid"));
					PC->ClientTravel(URL, TRAVEL_Absolute); 
				}
				else {
					UE_LOG(LogOnlineGame, Warning, TEXT("mroom is not valid"));
				}

			}
			else
			{
				UE_LOG(LogOnlineGame, Warning, TEXT("Failed to join session %s"), *InSessionName.ToString());
			}
		}
	}
} 
void  UUserWidget_MainMenu::MyOnFindRoom() { 
	UE_LOG(LogClass, Log, TEXT("mmainmenu find room"));     

	UWorld* MyWorld = GetWorld();
	if (MyWorld == nullptr) { 
		UE_LOG(LogClass, Log, TEXT("mmainmenu get null world"));
		return;
	}
	//IOnlineSubsystem* OnlineSub = Online::GetSubsystem(MyWorld);
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{ 
		UE_LOG(LogClass, Log, TEXT("mmainmenu get onlinesub"));
		//IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		IOnlineSessionPtr Sessions = Online::GetSessionInterface(MyWorld);
		if (Sessions.IsValid()) { 

			bool bIsLan = true;

			SearchSettings = MakeShareable(new FOnlineSessionSearch());
			//SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			SearchSettings->bIsLanQuery = true;
			//Sessions->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UUserWidget_MainMenu::OnFindSessionsComplete));
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UUserWidget_MainMenu::OnFindSessionsComplete));
			

			TArray<ULocalPlayer*> lps = UGameplayStatics::GetGameInstance(GetWorld())->GetLocalPlayers();
			 
			if (lps.Num() > 0) {
				TSharedPtr<const FUniqueNetId> uni = lps[0]->GetPreferredUniqueNetId().GetUniqueNetId();
				 

				bool ok = Sessions->FindSessions(*uni, SearchSettings.ToSharedRef());
				if (ok) {
					UE_LOG(LogClass, Log, TEXT("mmainmenu find sessions ok"));

				}
				else {
					UE_LOG(LogClass, Log, TEXT("mmainmenu find sessions not ok"));
				}
			}
		} else { 
			UE_LOG(LogClass, Log, TEXT("mmainmenu onliesession interface not valid"));
		}
		 
	}
}


void UUserWidget_MainMenu::OnFindSessionsComplete(bool bWasSuccessful) {
	UE_LOG(LogClass, Log, TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful);

	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			UE_LOG(LogClass, Log, TEXT("Num Search Results: %d"), SearchSettings->SearchResults.Num());
			for (int32 SearchIdx = 0; SearchIdx < SearchSettings->SearchResults.Num(); SearchIdx++)
			{
				const FOnlineSessionSearchResult& SearchResult = SearchSettings->SearchResults[SearchIdx];
				DumpSession(&SearchResult.Session);
			}
			 
		}
	}  
}

void UUserWidget_MainMenu::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	UE_LOG(LogClass, Log, TEXT("OnCreateSessionComplete %s bSuccess: %d"), *SessionName.ToString(),bWasSuccessful);

	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		if (bWasSuccessful) {
			UGameplayStatics::OpenLevel(GetWorld(), "ThirdPersonExampleMap", true, TEXT("listen"));
		}
	}
}
void UUserWidget_MainMenu::MyOnCreateRoom() {
	UE_LOG(LogClass, Log, TEXT("mmainmenu create room"));

	UWorld* MyWorld = GetWorld();
	if (MyWorld == nullptr) {
		UE_LOG(LogClass, Log, TEXT("mmainmenu get null world"));
		return;
	}
	//IOnlineSubsystem* OnlineSub = Online::GetSubsystem(MyWorld);
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		UE_LOG(LogClass, Log, TEXT("mmainmenu get onlinesub"));
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface(); 
		if (Sessions.IsValid()) { 
			
			TArray<ULocalPlayer*> lps = UGameplayStatics::GetGameInstance(GetWorld())->GetLocalPlayers();
			TSharedPtr<const FUniqueNetId> uni;
			if (lps.Num() > 0) {  
				uni = lps[0]->GetPreferredUniqueNetId().GetUniqueNetId();
				FOnlineSessionSettings oss;
				oss.NumPublicConnections = 3;
				oss.NumPrivateConnections = 3;
				oss.bIsLANMatch = true;
				oss.bIsDedicated = false;
				oss.bAllowInvites = true;
				oss.bAllowJoinInProgress = true;
				oss.bAllowJoinViaPresence = true;
				oss.bShouldAdvertise = true;

				// pass session name
				FOnlineSessionSetting compoundSessionName;
				compoundSessionName.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
				compoundSessionName.Data = ToCreateRoomName;
				oss.Settings.Add(FName(SESSION_NAME), compoundSessionName);


				OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UUserWidget_MainMenu::OnCreateSessionComplete));

				bool ok = Sessions->CreateSession(*uni, FName(ToCreateRoomName), oss);
				if (ok) {

					UE_LOG(LogClass, Log, TEXT("mmainmenu create session  ok"));
				}
				else {

					UE_LOG(LogClass, Log, TEXT("mmainmenu create session not ok"));
				}
			}
			
			   
		}
		else {
			UE_LOG(LogClass, Log, TEXT("mmainmenu onliesession interface not valid"));
		}

	}
}

void UUserWidget_MainMenu::MyOnJoinButtonClicked()
{
	MyJoinSession(ToJoinRoomIndex);
}

void UUserWidget_MainMenu::SetToJoinRoomIndex(int32 x)
{
	ToJoinRoomIndex = x;
}

int32 UUserWidget_MainMenu::GetToJoinRoomIndex()
{
	return ToJoinRoomIndex;
}

FString UUserWidget_MainMenu::GetRoomInfoText() {
	FString ans = "";
	if (SearchSettings != nullptr) {
		int c = SearchSettings->SearchResults.Num();
		ans = ans + FString::Printf(TEXT("Session Count:%d"), c) + "\n";
		for (int i = 0; i < c; i++) {
			ans = ans + FString::Printf(TEXT("Index:%d\n"),i);
			auto tmpx = SearchSettings->SearchResults[i].Session.SessionSettings.Settings.FindRef(FName(SESSION_NAME));
			auto tmpy = tmpx.Data;
			auto tmpz = tmpy.ToString();
			if (!tmpz.IsEmpty()) {
				ans = ans + FString::Printf(TEXT("    SessionName:%s\n"), *tmpz);
			}
		}
	}
	else {
		ans = "Not already search";
	}
	return ans;
}