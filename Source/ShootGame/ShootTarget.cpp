// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootTarget.h"
#include "MyGameState.h"
 

// Sets default values
AShootTarget::AShootTarget()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootTargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootTargetMesh"));
	TargetScore = 2.0f;

	RootComponent = ShootTargetMesh;

	GetMesh()->SetSimulatePhysics(true); 
}

// Called when the game starts or when spawned
void AShootTarget::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShootTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShootTarget::WasShot()
{
	Destroy();
}

float AShootTarget::GetScore()
{
	float ans = TargetScore;
	AMyGameState* amgs = Cast<AMyGameState>(GetWorld()->GetGameState());
	if (amgs != nullptr) {
		if (amgs->GetRemainingTime() > 0) {
			uint32 x = amgs->GetRemainingTime();
			// such as 60
			ans = 2.0 * x / 10;
		}
		else {
			ans = 0;
		}
	}
	return ans;
}

