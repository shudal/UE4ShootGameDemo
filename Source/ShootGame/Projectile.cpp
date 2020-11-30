// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ShootTarget.h"
#include "ShootGameCharacter.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h"
#include "UObject/ConstructorHelpers.h" 
#include "Kismet/KismetMathLibrary.h"
#include "MyPlayerState.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;


	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	float tmpx = 2000.0f;
	ProjectileMovementComponent->InitialSpeed = tmpx;
	ProjectileMovementComponent->MaxSpeed = tmpx;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	LifeStarted = false;
	MIN_VELOCITY_TO_EXPLODE = 0.1; 
	EXPLODE_RADIUS = 2;
	EXPLODE_STRENGTH = 100;
	HARM_TO_MAN = 40;
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

	float MyVelo = GetVelocity().Size();

	//UE_LOG(LogClass, Log, TEXT("mphy projectile speed:%f"), MyVelo);
	if (LifeStarted && MyVelo <= MIN_VELOCITY_TO_EXPLODE) {
		MyBoom();
	}
}

void AProjectile::SetWeaponPlayer(ACharacter* NewPlayer)
{
	player = NewPlayer;
}

void AProjectile::OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogClass, Log, TEXT("xxxxx hit"));

	bool EverDesed = false;

	// °Ð×Ó
	AShootTarget* st = Cast<AShootTarget>(OtherActor);
	if (st != nullptr) {
		UE_LOG(LogClass, Log, TEXT("xxxxx hit target"));
		if (player != nullptr) {
			AShootGameCharacter* playerChar = Cast<AShootGameCharacter>(player);
			playerChar->UpdateScore(st->GetScore());
		}
	}

	
	// ½ÇÉ«
	ACharacter* hitedchar = Cast<ACharacter>(OtherActor);
	if (hitedchar != nullptr) {
		if (GetLocalRole() == ROLE_Authority) {
			AShootGameCharacter* sgc = Cast<AShootGameCharacter>(hitedchar);
			if (sgc != nullptr) {
				AMyPlayerState* mps = Cast<AMyPlayerState>(sgc->GetPlayerState());
				if (mps != nullptr) {
					ECharLifeType pres = mps->GetLifeState();
					sgc->UpdateBlood(-1 * HARM_TO_MAN);
					//mps->SetBlood(mps->GetBlood() - HARM_TO_MAN);
					auto afters = mps->GetLifeState();
					if (pres == ECharLifeType::CLT_ALIVE && afters == ECharLifeType::CLT_DEAD) {
						auto mychar = Cast<AShootGameCharacter>(player);
						if (mychar != nullptr) {
							mychar->UpdateKillCount(1);
						}
					}
				}
			}
		} 
		MyBoom();
		EverDesed = true;
	}

	if (EverDesed == false) {

	}
}

void AProjectile::SetLifeStarted(bool x) {
	LifeStarted = x;
}

void AProjectile::MyBoom()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);


	auto MyWorld = GetWorld();
	if (MyWorld != nullptr) {

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = false;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		FCollisionObjectQueryParams coqp;
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FCollisionShape cs = FCollisionShape::MakeSphere(EXPLODE_RADIUS);

		TArray<FOverlapResult> Overlaps;
		bool everOverlaped = MyWorld->OverlapMultiByObjectType(Overlaps, GetActorLocation(), FQuat(), coqp, cs, RV_TraceParams);
		
		FVector ExplodeOriLoc = GetActorLocation();
		UE_LOG(LogClass, Log, TEXT("mphy ever overlaped:%d"), everOverlaped?1:0);
		if (everOverlaped) {
			for (int i = 0; i < Overlaps.Num(); i++) {
				FOverlapResult & ov = Overlaps[i];
				AActor* ac = ov.GetActor();

				if (ac != nullptr) { 
					TArray<UStaticMeshComponent*> Components;
					ac->GetComponents<UStaticMeshComponent>(Components);
					
					FVector Forc = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(ExplodeOriLoc, ac->GetActorLocation()));
					Forc.Normalize();
					Forc *= EXPLODE_STRENGTH;

					UE_LOG(LogClass, Log, TEXT("mphy overlaped components count:%d"), Components.Num());
					for (int32 k = 0; k < Components.Num(); k++)
					{
						UStaticMeshComponent* smc = Components[k];
						
						UE_LOG(LogClass, Log, TEXT("mphy overlaped components index %d, simulate physuics? %d"), k, smc->IsSimulatingPhysics() ? 1 : 0);
						if (smc->IsSimulatingPhysics()) {
							smc->AddImpulse(Forc);

						}
						//smc->AddForce(Forc);
						//smc->AddRadialForce(ExplodeOriLoc, EXPLODE_RADIUS, EXPLODE_STRENGTH, ERadialImpulseFalloff::RIF_Linear);
					}
				}
			}
		}
	}

	Destroy();
}
