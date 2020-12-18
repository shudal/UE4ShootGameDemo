// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_CharsContext.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "../ShootGameCharacter.h"
#include "../MyPlayerState.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UEQC_CharsContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const{

	TArray<FVector> outList;
	TArray<AActor*> findList;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AShootGameCharacter::StaticClass(), FName("player"), findList);
	for (int i = 0; i < findList.Num(); i++)
	{ 
		AShootGameCharacter* c = Cast<AShootGameCharacter>(findList[i]);
		if (c) {
			AMyPlayerState* s = c->GetPlayerState<AMyPlayerState>();
			if (s) {
				if (s->GetLifeState() == ECharLifeType::CLT_ALIVE) { 
					outList.Add(findList[i]->GetActorLocation());
				}
			}
		}
	} 
	if (outList.Num() == 0)
	{
		if (findList.Num() > 0) {
			outList.Add(findList[0]->GetActorLocation());
		}
		else {
			outList.Add(FVector(-25,-263,423));
		}
	} 
	UEnvQueryItemType_Point::SetContextHelper(ContextData, outList);
}
