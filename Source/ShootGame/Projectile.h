// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SHOOTGAME_API AProjectile : public AActor
{
	GENERATED_BODY()

		// 武器类 的 弹丸类

public:
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACharacter* player;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		float MIN_VELOCITY_TO_EXPLODE;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		float EXPLODE_RADIUS;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		float EXPLODE_STRENGTH;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		float HARM_TO_MAN;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return MyMesh; }

	void SetWeaponPlayer(ACharacter*);

	// 在投射物撞击其他对象并爆炸时使用的粒子。
	UPROPERTY(EditAnywhere, Category = "Effects")
	class UParticleSystem* ExplosionEffect;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;
	 
	void SetLifeStarted(bool x);

	void MyBoom();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MyMesh", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* MyMesh;

	UFUNCTION()
	void OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	bool LifeStarted;
};
