// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ShootTarget.h"
#include "ShootGameCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh; 
	 
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->OnComponentHit.AddDynamic(this, &AProjectile::OnMyHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::SetWeaponPlayer(ACharacter* NewPlayer)
{
	player = NewPlayer;
}
 
void AProjectile::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogClass, Log, TEXT("xxxxx hit"));

	AShootTarget* st = Cast<AShootTarget>(OtherActor);
	if (st != nullptr) {
		UE_LOG(LogClass, Log, TEXT("xxxxx hit target"));
		if (player != nullptr) {
			AShootGameCharacter* playerChar = Cast<AShootGameCharacter>(player);
			playerChar->UpdateScore(st->GetScore());
		}
	}

	Destroy();
}
