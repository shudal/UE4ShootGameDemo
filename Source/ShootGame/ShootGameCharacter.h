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

	UFUNCTION(BlueprintCallable)
	void SetIsFreeView(bool x);
	DECLARE_DELEGATE_OneParam(FSetIsFreeViewDelegate, bool);

	/*
	
	UFUNCTION(Server, Reliable)
		void HandleFire();
	void HandleFire_Implementation();

	*/

	UFUNCTION(Server, Reliable)
		void SetSkillName(const FString& x); 
	void SetSkillName_Implementation(const FString& x);


	void SetAnimRotator(FRotator x); 

	  
	UFUNCTION(BlueprintCallable)
	void SetSkillNameWrapper(FString x);
	DECLARE_DELEGATE_OneParam(FStringDelegate, FString);
	UFUNCTION(BlueprintCallable)
		FString GetSkillName();


	UFUNCTION(BlueprintCallable)
	bool IsFreeView();


	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	UPROPERTY(ReplicatedUsing=OnRep_FakeAnimRotator)
	FRotator FakeAnimRotator;
	UFUNCTION()
	void OnRep_FakeAnimRotator();
	int32 xcount;
	



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	bool bIsFreeView;

	UPROPERTY(Replicated)
	FString MySkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	bool bIsAI;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	bool bMyMeleeDown;

	class AMyPlayerState* MyPlayerState;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = "ShootGame")
	float GetPlayerScore();

	void UpdateScore(float);


		void HandleFire(); 
	UFUNCTION(Server, Reliable)
		void ServerHandleFire();
	void ServerHandleFire_Implementation();
	UFUNCTION(NetMulticast, Reliable)
		void ClientHandleFire();
	void ClientHandleFire_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Shoot")
	void  HandleMeleeDown();
	UFUNCTION(BlueprintCallable, Category = "Shoot")
		void  HandleMeleeUp();
	UFUNCTION(BlueprintCallable)
		bool  IsMelee();

	UFUNCTION(BlueprintCallable, Category = "Shoot")
		void ShootTarget();

	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Shoot")
	bool IsAI();
	//UFUNCTION()
	//	void OnMyHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle TimerHandle_DefaultTimer;

	void DefaultTimer();


	UFUNCTION(BlueprintCallable, Category = "Shoot")
	void PickupThing();


	UFUNCTION(Server, Reliable)
	void ServerPickupThing();  
	void ServerPickupThing_Implementation();
	UFUNCTION(NetMulticast, Reliable)
	void ClientPickupThing();
	void ClientPickupThing_Implementation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGame")
	float PICK_DISTANCE = 100;


	UFUNCTION(BlueprintCallable, Category = "Shoot")
	void ChangeWeapon();


	UFUNCTION(Server, Reliable)
		void ServerChangeWeapon();
	void ServerChangeWeapon_Implementation();
	UFUNCTION(NetMulticast, Reliable)
		void ClientChangeWeapon();
	void ClientChangeWeapon_Implementation();
private:
	void HoldWeapon();

	AWeapon* MyWeaponClass = nullptr;

	class AMyPlayerState* GetMyPlayerState();
};

