// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SHOOTGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

		// ÎäÆ÷Àà µÄ µ¯ÍèÀà

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* player;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return MyMesh; }

	void SetWeaponPlayer(ACharacter*);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MyMesh;

	UFUNCTION()
	void OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
