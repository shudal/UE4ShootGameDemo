// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "ShootGameCharacter.h"

#include "Kismet/KismetMathLibrary.h"
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
	if (player != nullptr) {
		MyShootGameChar = Cast<AShootGameCharacter>(player);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MyShootGameChar != nullptr) {
		//SetActorRelativeRotation(UKismetMathLibrary::MakeRotator(0, MyShootGameChar->GetMyAnimRotator().Pitch, MyShootGameChar->GetMyAnimRotator().Yaw + 280));
	}
	else { 
		MyShootGameChar = Cast<AShootGameCharacter>(player); 
	}
}

void AWeapon::WeaponFire()
{
	if (MyProjectile != NULL) {
		UWorld* const World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			FVector SpawnLoc;
			FRotator SpawnRot;  

			SpawnLoc = this->GetMesh()->GetSocketLocation("ProjectileSocket");
			
			if (MyShootGameChar != nullptr) {
				SpawnRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation("ProjectileSocket"), GetMesh()->GetSocketLocation("ProjectileSocket2"));
				//SpawnRot = MyArrow->GetComponentRotation();
				//SpawnRot = GetActorRotation() + MyArrow->GetComponentRotation();
				//SpawnRot = UKismetMathLibrary::RotatorFrom GetMesh()->GetSocketLocation("ProjectileSocket2") - GetMesh()->GetSocketLocation("ProjectileSocket");
				//SpawnRot = MyShootGameChar->GetActorRotation() +  MyShootGameChar->GetMyAnimRotator();
				//SpawnRot = MyShootGameChar->GetActorRotation() +  MyShootGameChar->GetMesh()->GetSocketRotation("ring_02_r_socket");
			}

			 
			AProjectile* tmp = World->SpawnActor<AProjectile>(MyProjectile, SpawnLoc, SpawnRot, SpawnParams);

			if (tmp != nullptr) {
				tmp->SetWeaponPlayer(player);
				//tmp->AttachToComponent(this->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), "ProjectileSocket");
				/* 
				UE_LOG(LogClass, Log, TEXT("projectile not null"));
				if (MyArrow != nullptr) {
					UE_LOG(LogClass, Log, TEXT("myarrow is not null")); 
					UE_LOG(LogClass, Log, TEXT("%f %f %f"), force.X, force.Y, force.Z); 
				}
				else {
					UE_LOG(LogClass, Log, TEXT("myarrow is null"));
				}
				*/
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
