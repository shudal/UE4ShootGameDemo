// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Weapon.generated.h"


UCLASS()
class SHOOTGAME_API AWeapon : public AActor 
{
	GENERATED_BODY()

		// 武器类
public:
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		class UArrowComponent* MyArrow;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AProjectile> MyProjectile;

	  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ProjectileInitialDistance;

	ACharacter* player;
	class AShootGameCharacter* MyShootGameChar;
	 
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return MyMesh; }

	void WeaponFire();
	void SetWeaponPlayer(ACharacter*);

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(Server, Reliable)
		void HandleFire();
	void HandleFire_Implementation();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MyMesh;
};
