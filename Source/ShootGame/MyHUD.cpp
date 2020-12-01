// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "ShootGameCharacter.h"
#include "Misc/App.h"
#include "Components/ListView.h"
#include "MyGameState.h"
#include "Components/TextBlock.h"
#include "MySaveGame.h" 

UMyHUD::UMyHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MListView = nullptr;
}
bool UMyHUD::Initialize() { 
	if (!Super::Initialize()) {
		return false;
	} 
	if (UListView* ulv = Cast<UListView>(GetWidgetFromName("MyListView"))) {
		MListView = ulv;

		UE_LOG(LogClass, Log, TEXT("mlistview 4, list item count:%d"), MListView->GetNumItems());
		MListView->ClearListItems();
	}
	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("MyTextInUI"))) {
		TB_RelifeTIp = utb;
		TB_RelifeTIp->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UTextBlock* utb = Cast<UTextBlock>(GetWidgetFromName("TB_PlayerStateList"))) {
		TB_PlayerStateList = utb; 
	}
	

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &UMyHUD::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	MyGameState = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));
	SetMyPlayerState();


	// ---
	if (UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("MySaveGame", 0)))
	{
		// 操作成功，LoadedGame现在将包含较早前保存的数据。 

		UE_LOG(LogClass, Log, TEXT("LOADED:%s"), *(LoadedGame->GetPlayerName()));
		TArray<FMyScoreItemDataStruct> PlayerRank = LoadedGame->GetPlayerRank();

		UE_LOG(LogClass, Log, TEXT("msavegame player rank loaded count:%d"), PlayerRank.Num());
		for (int32 i = 0; i < PlayerRank.Num(); i++) {
			auto x = PlayerRank[i];
			UE_LOG(LogClass, Log, TEXT("msavegame: loaded: playerid:%d,playername:%s,playerscore:%f,playerrank:%d"), x.PlayerId, *(x.PlayerName), x.PlayerScore, x.PlayerRank);
		}

		if (mps != nullptr) {
			mps->SetPlayerNick(LoadedGame->GetPlayerName());
		}

	}
	else {

		UE_LOG(LogClass, Log, TEXT("msavegame loaded not ok"));
	}
	// ---

	return true;
}
void UMyHUD::DefaultTimer() {
	// 
	if (mps != nullptr) {
		auto ls = mps->GetLifeState();
		if (ls == ECharLifeType::CLT_DEAD) {
			if (TB_RelifeTIp != nullptr) {
				TB_RelifeTIp->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}


	UE_LOG(LogClass, Log, TEXT("mlistview 1"));
	if (MyGameState != nullptr) {

		UE_LOG(LogClass, Log, TEXT("mlistview 2"));
		if (MyGameState->IsGameFinished()) { 

			if (MListView->GetNumItems() == 0) {
				UE_LOG(LogClass, Log, TEXT("mlistview 3"));
				SetScoreList();
			}

			// --

			if (UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())))
			{
				// 设置savegame对象上的数据。
				if (MyGameState != nullptr) {

					SaveGameInstance->SetPlayerRank(MyGameState->GetScoreItemArray());
					TArray<FMyScoreItemDataStruct> PlayerRank = SaveGameInstance->GetPlayerRank();
					UE_LOG(LogClass, Log, TEXT("msavegame saved count1:%d"), PlayerRank.Num());
					for (int32 i = 0; i < PlayerRank.Num(); i++) {
						auto x = PlayerRank[i];
						UE_LOG(LogClass, Log, TEXT("msavegame: saved: playerid:%d,playername:%s,playerscore:%f,playerrank:%d"), x.PlayerId, *(x.PlayerName), x.PlayerScore, x.PlayerRank);

					}
					UE_LOG(LogClass, Log, TEXT("msavegame saved count2:%d"), SaveGameInstance->GetPlayerRank().Num());

					// 从自己的player state中得到自己的昵称 
					if (mps != nullptr) {
						SaveGameInstance->SetPlayerName(mps->GetPlayerNick());
					}

					// 即时保存游戏。
					if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->GetSaveSlotName(), SaveGameInstance->GetUserIndex()))
					{
						int32 plid = -1;
						if (mps != nullptr) {
							plid = mps->GetPlayerId();
						}
						// 成功保存。 
						UE_LOG(LogClass, Log, TEXT("msavegame save ok,playerid:%d,playerNick:%s"), plid, *(SaveGameInstance->GetPlayerName()));
					}
					else {
						UE_LOG(LogClass, Log, TEXT("msavegame save not ok"));
					}
				}
			}
			// --
		}
		else {
		}
	}
}
FString UMyHUD::GetScoreText() {
	FString  ans = TEXT("HI");

	return ans;
}
FString UMyHUD::GetPersonalInfoText() {
	FString  ans;


	//FString OSLabel, OSVersion;
	//FPlatformMisc::GetOSVersions(OSLabel, OSVersion);
	//FString CPUInfo = FPlatformMisc::GetCPUBrand();
	//FString GPUInfo = FPlatformMisc::GetPrimaryGPUBrand();

	int32 PlayerId = -1;


	FString PlayerNick = TEXT("NoneName");
	if (mps != nullptr) {
		PlayerId = mps->GetPlayerId();
		PlayerNick = mps->GetPlayerNick();
	}
	ans = ans + FString::Printf(TEXT("Personal Character Info:\nPlayer Id: %d\nPlayerNick: %s\n"), PlayerId, *PlayerNick);

	if (mps != nullptr) { 
		// weapon info
		int32 len = mps->GetWeaponData().Num();
		ans = ans + FString::Printf(TEXT("Weapon Count:%d\n"), len);
		if( len > 0) { 
			ans = ans + FString::Printf(TEXT("Now Weapon Name:%s\n"), *(mps->GetWeaponData()[mps->GetNowWeaponIndex()]).WeaponName);
		}

		// blood info
		float blo = mps->GetBlood();	
		ans = ans + FString::Printf(TEXT("Blood:%f\n"), blo);

	}
	else {
		SetMyPlayerState();
	}
	return ans;
}
float UMyHUD::GetMyScore()
{
	float ans = -1.0f;
	if (mps != nullptr) {
		ans = mps->GetPlayerScore();
	}
	else {
		this->SetMyPlayerState();
	}
	return ans;
}

void UMyHUD::SetMyPlayerState() { 

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShootGameCharacter::StaticClass(), FoundActors);
	for (auto Actor : FoundActors) {
		AShootGameCharacter* MyChar = Cast<AShootGameCharacter>(Actor);
		if (MyChar != nullptr && MyChar->IsLocallyControlled()) {
			this->mchar = MyChar;

			AMyPlayerState* getmps = Cast<AMyPlayerState>(MyChar->GetPlayerState());
			if (getmps) {
				this->mps = getmps;
			}
		} 
	}
}

void UMyHUD::MyPlayerShoot() {
	if (mchar != nullptr) {
		mchar->ShootTarget();
	}
	else {
		this->SetMyPlayerState();
	}
}
void UMyHUD::SetScoreList() {  
	if (MListView == nullptr) { return; } 
	MListView->ClearListItems();
	 
	int i = 1;

	AMyGameState* gs = Cast<AMyGameState>(UGameplayStatics::GetGameState(GetWorld()));

	if (gs != nullptr) {
		TArray<UMyScoreItemData*> sids = gs->GetScoreItemArray();
		for (UMyScoreItemData* si : sids) { 
			si->SetPlayerRank(i++);
			MListView->AddItem(si);

			UE_LOG(LogClass, Log, TEXT("mlistview playerScore:%f"), si->GetPlayerScore());
		}
	}
}