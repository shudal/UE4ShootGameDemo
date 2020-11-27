// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	WT_Projectile,
WT_Ray
};

UENUM(BlueprintType)
enum class EWeaponKind : uint8 {
	WK_RayGun,
	WK_ProjectileGun
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()


		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString WeaponName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponKind MyWeaponKind;

	// EWeaponKind和WeaponStaticClass是一一对应关系
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		TSubclassOf<class AWeapon> WeaponStaticClass;


	bool operator==(const FWeaponData& rhs) const {
		if (MyWeaponKind == rhs.MyWeaponKind) {
			return true;
		}
		else {
			return false;
		}
	}

	FString ToString() { 
		FString s = FString::Printf(TEXT("WeaponName:%s,WeaponKind:%d"), *WeaponName, (uint8)MyWeaponKind);
		return s;
	}
};

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
	 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType MyWeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FWeaponData MyWeaponData;
	 
	UParticleSystem* ShootEffect;
	UParticleSystemComponent* ShootEffectComponent;
	USoundBase* ShootImpactMeshSound; 
	USoundBase* ShootImpactDefaultSound;
	USoundBase* ShootImpactMetalSound;
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
	 

	UFUNCTION(BlueprintCallable)
	void ProcessHitResult(const TArray<FHitResult>& HitResults);


	FTimerHandle TimerHandle_DefaultTimer;

	void DefaultTimer();
	 
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MyMesh;
	 
	TArray<UParticleSystemComponent*> SpawnedEffects;
};

 