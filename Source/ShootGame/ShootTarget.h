// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootTarget.generated.h"

// °Ð×ÓÀà

UCLASS()
class SHOOTGAME_API AShootTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShootTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float TargetScore;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ShootTarget")
		void WasShot();

	float GetScore();

	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ShootTargetMesh; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShootTarget", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ShootTargetMesh;
};
