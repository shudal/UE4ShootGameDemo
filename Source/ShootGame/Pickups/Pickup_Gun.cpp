// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Gun.h"
#include "Components/BoxComponent.h"

#include "Kismet/GameplayStatics.h"

APickup_Gun::APickup_Gun() {

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh")); 
	RootComponent = MyMesh;
}
FWeaponData APickup_Gun::GetWeaponData() {
	return MyWeaponData;
}


void APickup_Gun::BeginPlay() {
	Super::BeginPlay();

	SetIsActive(true);

	//MyMesh->SetSimulatePhysics(true); 
}

void APickup_Gun::BePicked() {
	if (BePickedSound != nullptr) { 
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BePickedSound, GetActorLocation());
	}
	this->Destroy(); 
}