// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "ShootGameCharacter.generated.h"

UCLASS(config=Game)
class AShootGameCharacter : public ACharacter
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AShootGameCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	FVector GetFaceDirection();


	UFUNCTION(BlueprintCallable)
	void SetMyAnimYaw(float x);

	UFUNCTION(BlueprintCallable)
	FRotator GetMyAnimRotator();
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	float MyAnimYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
		float MyAnimPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	FRotator MyAnimRotator;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<AWeapon> MyWeapon;

	void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rot")
		float rx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rot")
		float ry;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rot")
		float rz;

	float PlayerScore;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = "ShootGame")
	float GetPlayerScore();

	void UpdateScore(float);

	/** 用于生成投射物的服务器函数。*/
	UFUNCTION(Server, Reliable)
		void HandleFire();
	void HandleFire_Implementation();



	UFUNCTION(BlueprintCallable, Category = "Shoot")
		void ShootTarget();

	void Tick(float DeltaTime) override;
private:
	void HoldWeapon();

	AWeapon* MyWeaponClass = nullptr;
};

