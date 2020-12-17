#include "FarShootBot.h"
#include "../MyPlayerState.h"

void AFarShootBot::BeginPlay() {
	Super::BeginPlay();
	 
	if (GetWorld()) {
		UE_LOG(LogClass, Log, TEXT("mbot add weapon tiemr ok"));
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AFarShootBot::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);
	}
	else { 
		UE_LOG(LogClass, Log, TEXT("mbot add weapon tiemr failed"));
	}
}

void AFarShootBot::DefaultTimer()
{
	Super::DefaultTimer();

	if (bEverIniWeapon == false) {  
		if (IniWeapon.WeaponStaticClass != nullptr) {
			if (GetMyPlayerState() != nullptr) {
				bEverIniWeapon = true;
				GetMyPlayerState()->AddWeapon(IniWeapon);
				this->ChangeWeapon();
				UE_LOG(LogClass, Log, TEXT("mbot add weapon ok"));

			}
			else {
				UE_LOG(LogClass, Log, TEXT("mbot add weapon failed,player state is null"));

			}
		}
	}
}
