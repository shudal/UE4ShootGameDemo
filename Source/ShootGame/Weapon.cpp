// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	MyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MyArrow"));
	if (MyArrow == nullptr) {
		UE_LOG(LogClass, Log, TEXT("xxxxxmy arrow initialize to null"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("xxxxxmy arrow initialize to  not null")); 
	}
	RootComponent = MyMesh;
	 
	ProjectileInitialDistance = 30.0f;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::WeaponFire()
{
	if (MyProjectile != NULL) {
		UWorld* const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			FVector SpawnLoc = GetActorLocation();
			FRotator SpawnRot = player->GetControlRotation();
			 
			FRotator rot1 = GetActorRotation();
			UE_LOG(LogClass, Log, TEXT("pitch %f yaw %f roll %f"), rot1.Pitch, rot1.Yaw, rot1.Roll);
			FRotator rot = MyArrow->GetComponentRotation();
			UE_LOG(LogClass, Log, TEXT("pitch %f yaw %f roll %f"), rot.Pitch, rot.Yaw, rot.Roll);
			// actor的rotaion + 箭头的componentRotation就是最终的
			rot = rot + rot1;

			FVector force = rot.Vector();
			SpawnLoc = SpawnLoc + force * ProjectileInitialDistance;

			AProjectile* tmp = World->SpawnActor<AProjectile>(MyProjectile, SpawnLoc, SpawnRot, SpawnParams);

			if (tmp != nullptr) {
				UE_LOG(LogClass, Log, TEXT("projectile not null"));
				if (MyArrow != nullptr) {
					UE_LOG(LogClass, Log, TEXT("myarrow is not null")); 
					UE_LOG(LogClass, Log, TEXT("%f %f %f"), force.X, force.Y, force.Z); 
					tmp->SetWeaponPlayer(player);  
					//tmp->AttachToActor(player, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));
					 
					//tmp->FireInDirection(force);
					//tmp->GetMesh()->AddForce((force) * 1000.0f);
				}
				else {
					UE_LOG(LogClass, Log, TEXT("myarrow is null"));
				}
			}

		}
	}
}


void AWeapon::SetWeaponPlayer(ACharacter* NewPlayer) {
	player = NewPlayer;
}

void AWeapon::HandleFire_Implementation()
{
}
