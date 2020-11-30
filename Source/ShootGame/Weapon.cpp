// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "ShootGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Particles/ParticleSystem.h" 
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyPlayerState.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	  
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	RootComponent = MyMesh; 
	MyArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MyArrow"));
	if (MyArrow == nullptr) {
		UE_LOG(LogClass, Log, TEXT("xxxxxmy arrow initialize to null"));
	}
	else {
		UE_LOG(LogClass, Log, TEXT("xxxxxmy arrow initialize to  not null")); 
	}
	 
	ProjectileInitialDistance = 30.0f;
	MyWeaponType = EWeaponType::WT_Projectile;
	
	ShootEffect = nullptr;
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultShootEffect(TEXT("/Game/ExampleContent/Effects/ParticleSystems/P_electricity_arc.P_electricity_arc"));
	if (DefaultShootEffect.Succeeded()) {
		ShootEffect = DefaultShootEffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultImpactSound(TEXT("/Game/Sounds/Impacts/SCue_Impt_Bullet_Default.SCue_Impt_Bullet_Default"));
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultImpactMeshSound(TEXT("/Game/Sounds/Impacts/SCue_Impt_Bullet_Flesh.SCue_Impt_Bullet_Flesh"));
	static ConstructorHelpers::FObjectFinder<USoundBase> DefaultImpactMetalSound(TEXT("/Game/Sounds/Impacts/SCue_Impt_Bullet_Metal.SCue_Impt_Bullet_Metal"));

	if (DefaultImpactSound.Succeeded()) {
		ShootImpactDefaultSound = DefaultImpactSound.Object;
	}
	if (DefaultImpactMeshSound.Succeeded()) {
		ShootImpactMeshSound = DefaultImpactMeshSound.Object;
	}
	if (DefaultImpactMetalSound.Succeeded()) {
		ShootImpactMetalSound = DefaultImpactMetalSound.Object;
	}

	RAY_GUN_HARM_TO_MAN = 40;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (player != nullptr) {
		MyShootGameChar = Cast<AShootGameCharacter>(player);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AWeapon::DefaultTimer, GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation(), true);

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
	if (MyWeaponType == EWeaponType::WT_Projectile) { 
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
					tmp->SetLifeStarted(true);
					
				}

			}
		}
	}
	else if (MyWeaponType == EWeaponType::WT_Ray) {
		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = false; 
		RV_TraceParams.bReturnPhysicalMaterial = true; 

		//Re-initialize hit info
		//FHitResult RV_Hit(ForceInit);

		FCollisionObjectQueryParams coqp;
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		coqp.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

		FVector Start =  this->GetMesh()->GetSocketLocation("ProjectileSocket");
		auto Rot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation("ProjectileSocket"), GetMesh()->GetSocketLocation("ProjectileSocket2"));
		FVector End = Start + UKismetMathLibrary::GetForwardVector(Rot) * 1500;
		 
	
		TArray<FHitResult> HitResults;
		bool everHit = GetWorld()->LineTraceMultiByObjectType(
			HitResults,
			Start,
			End,
			coqp,
			RV_TraceParams
		);
		/* 
		bool everHit = GetWorld()->LineTraceSingleByObjectType(
			RV_Hit,
			Start,
			End,
			coqp,
			RV_TraceParams
		); 
		*/

		if (everHit) {
			ProcessHitResult(HitResults);
		}
		else {
			if (ShootEffect != nullptr) { 
				//UParticleSystemComponent* ShootEffectComponent;// = UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), "ProjectileSocket", Start, Rot, EAttachLocation::SnapToTargetIncludingScale);
				auto AnotherShootEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect, Start);
				if (AnotherShootEffectComponent != nullptr) {
					AnotherShootEffectComponent->SetBeamTargetPoint(0, End, 0);
					SpawnedEffects.Add(AnotherShootEffectComponent);
				}
			}

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootImpactDefaultSound, End);

		}
	}
}


void AWeapon::ProcessHitResult(const TArray<FHitResult>& HitResults) {
	bool EverHited = false;

	bool EverHitMan = false;
	AShootGameCharacter* hitedplayer = nullptr;

	for (int i = 0; i < HitResults.Num() && EverHited == false; i++) {
		auto HitRe = HitResults[i];

		bool bBlockingHit, bInitialOverlap;
		float Time, Distance;
		FVector Location, ImpactPoint, Normal, ImpactNormal;
		UPhysicalMaterial* PhysMat;
		AActor* HitActor;
		UPrimitiveComponent* HitComponent;
		FName HitBoneName;
		int32 HitItem, FaceIndex;
		FVector TraceStart, TraceEnd;
		UGameplayStatics::BreakHitResult(HitRe, bBlockingHit, bInitialOverlap, Time, Distance, Location, ImpactPoint, Normal, ImpactNormal
			,
			PhysMat,
			HitActor,
			HitComponent,
			HitBoneName,
			HitItem,
			FaceIndex,
			TraceStart,
			TraceEnd
		);
		 
		 

		 

		if (HitBoneName.IsNone() == false &&  HitActor != nullptr) {
			hitedplayer = Cast<AShootGameCharacter>(HitActor);
			UE_LOG(LogClass, Log, TEXT("mphy hit bone name:%s"), *HitBoneName.ToString());

			if (hitedplayer != nullptr) {
				EverHited = true;
				EverHitMan = true;

				if (ShootEffect != nullptr) {
					FVector Start = this->GetMesh()->GetSocketLocation("ProjectileSocket");
					auto Rot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation("ProjectileSocket"), GetMesh()->GetSocketLocation("ProjectileSocket2"));

					FVector End = Location;
					//UParticleSystemComponent* ShootEffectComponent;// = UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), "ProjectileSocket", Start, Rot, EAttachLocation::SnapToTargetIncludingScale);
					auto AnotherShootEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect, Start);
					if (AnotherShootEffectComponent != nullptr) {
						AnotherShootEffectComponent->SetBeamTargetPoint(0, End, 0);
						SpawnedEffects.Add(AnotherShootEffectComponent);
					}
				}
				FString pname = hitedplayer->GetName();
				UE_LOG(LogClass, Log, TEXT("mphy hit player name:%s"), *pname);

				bool everPlaySound = false;
				if (everPlaySound == false && HitBoneName == "head") {
					everPlaySound = true;
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootImpactMetalSound, Location);
				}
				if (everPlaySound == false && ShootImpactMeshSound != nullptr) {
					everPlaySound = true;
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootImpactMeshSound, Location);
				}

				if (everPlaySound == false) {
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootImpactDefaultSound, Location);
				}
			}
		}

		 
	}

	// 遍历完后，没有想射中的，就弄这个默认射中
	if (EverHited == false) {
		auto HitRe = HitResults[0];
		bool bBlockingHit, bInitialOverlap;
		float Time, Distance;
		FVector Location, ImpactPoint, Normal, ImpactNormal;
		UPhysicalMaterial* PhysMat;
		AActor* HitActor;
		UPrimitiveComponent* HitComponent;
		FName HitBoneName;
		int32 HitItem, FaceIndex;
		FVector TraceStart, TraceEnd;
		UGameplayStatics::BreakHitResult(HitRe, bBlockingHit, bInitialOverlap, Time, Distance, Location, ImpactPoint, Normal, ImpactNormal
			,
			PhysMat,
			HitActor,
			HitComponent,
			HitBoneName,
			HitItem,
			FaceIndex,
			TraceStart,
			TraceEnd
		);

		if (ShootEffect != nullptr) {
			FVector Start = this->GetMesh()->GetSocketLocation("ProjectileSocket");
			auto Rot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation("ProjectileSocket"), GetMesh()->GetSocketLocation("ProjectileSocket2"));

			FVector End = Location;
			//UParticleSystemComponent* ShootEffectComponent;// = UGameplayStatics::SpawnEmitterAttached(ShootEffect, GetMesh(), "ProjectileSocket", Start, Rot, EAttachLocation::SnapToTargetIncludingScale);
			auto AnotherShootEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootEffect, Start);
			if (AnotherShootEffectComponent != nullptr) {
				AnotherShootEffectComponent->SetBeamTargetPoint(0, End, 0);
				SpawnedEffects.Add(AnotherShootEffectComponent);
			}
		}

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ShootImpactDefaultSound, Location);
		
		hitedplayer = Cast<AShootGameCharacter>(HitActor);
		if (hitedplayer != nullptr) {
			EverHitMan = true;
		}
	}

	// 打到过人
	if (EverHitMan && hitedplayer != nullptr) { 
		if (GetLocalRole() == ROLE_Authority) { 
			AShootGameCharacter* sgc = hitedplayer;
			if (sgc != nullptr) {
				AMyPlayerState* mps = Cast<AMyPlayerState>(sgc->GetPlayerState());
				if (mps != nullptr) {
					ECharLifeType pres = mps->GetLifeState();
					sgc->UpdateBlood(-1 * RAY_GUN_HARM_TO_MAN);
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
	}
}
void AWeapon::DefaultTimer()
{

	while (SpawnedEffects.Num() > 0) {
		auto p = SpawnedEffects[0];
		p->DeactivaateNextTick();
		SpawnedEffects.RemoveAt(0);
	}

}
void AWeapon::BeginDestroy()
{
	Super::BeginDestroy();

	while (SpawnedEffects.Num() > 0) {
		auto p = SpawnedEffects[0];
		p->DeactivaateNextTick();
		SpawnedEffects.RemoveAt(0);
	}
}
void AWeapon::SetWeaponPlayer(ACharacter* NewPlayer) {
	player = NewPlayer;
}

void AWeapon::HandleFire_Implementation()
{
}
