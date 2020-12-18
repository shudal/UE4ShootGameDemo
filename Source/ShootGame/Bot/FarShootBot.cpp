#include "FarShootBot.h"
#include "../MyPlayerState.h"

AFarShootBot::AFarShootBot() {
	bEverIniWeapon = false;
}
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

	if (MyWeaponClass == nullptr) {
		if (GetLocalRole() != ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot myweaponclass is nullptr"));
		
		if (IniWeapon.WeaponStaticClass != nullptr) {
			if (GetLocalRole() != ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot myweaponclass init weaon data is not null"));

			AddWeapon(IniWeapon);
			ClientChangeWeapon();
		}
		else {
			if (GetLocalRole() != ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot myweaponclass init weaon data is null"));

		}
	}
	else {

		if (GetLocalRole() != ROLE_Authority) UE_LOG(LogClass, Log, TEXT("mbot myweaponclass is not nullptr"));
	}
}
