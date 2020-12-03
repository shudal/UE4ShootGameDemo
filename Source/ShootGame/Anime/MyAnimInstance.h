// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTGAME_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	class AShootGameCharacter* MyChar;
	class AShootGameCharacter* GetMyChar();

public:
	UMyAnimInstance();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_N_RightKick_Start(class UAnimNotify* Notify);


	UFUNCTION(BlueprintCallable)
		void AnimNotify_N_RightKick_End(class UAnimNotify* Notify);
};
