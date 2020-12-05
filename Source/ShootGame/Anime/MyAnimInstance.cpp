// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "../ShootGameCharacter.h"



UMyAnimInstance::UMyAnimInstance() {
	MyChar = nullptr;
}
void UMyAnimInstance::AnimNotify_N_RightKick_Start(UAnimNotify* Notify) {

	UE_LOG(LogClass, Log, TEXT("manim kick start"));
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetCharSkill(ECharSkill::RightKick);
		GetMyChar()->SetIsMeleeHarm(true);
	}
}

 
void UMyAnimInstance::AnimNotify_N_RightKick_End(UAnimNotify* Notify) {

	UE_LOG(LogClass, Log, TEXT("manim kick end")); 
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetIsMeleeHarm(false);
	}
}

void UMyAnimInstance::AnimNotify_N_LeftJab_Start(UAnimNotify* Notify)
{
	UE_LOG(LogClass, Log, TEXT("manim left jab start"));
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetCharSkill(ECharSkill::LeftTap);
		GetMyChar()->SetIsMeleeHarm(true);
	}
}

void UMyAnimInstance::AnimNotify_N_LeftJab_End(UAnimNotify* Notify)
{
	UE_LOG(LogClass, Log, TEXT("manim left jab end"));
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetIsMeleeHarm(false);
	}
}


void UMyAnimInstance::AnimNotify_N_PowerSmash_Start(class UAnimNotify* Notify) {

	UE_LOG(LogClass, Log, TEXT("manim power smash start"));
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetCharSkill(ECharSkill::PowerSmash);
		GetMyChar()->SetIsMeleeHarm(true);
	}
}


void UMyAnimInstance::AnimNotify_N_PowerSmash_End(class UAnimNotify* Notify) {

	UE_LOG(LogClass, Log, TEXT("manim power smash end"));
	if (GetMyChar() != nullptr) {
		GetMyChar()->SetIsMeleeHarm(false);
	}
}
AShootGameCharacter* UMyAnimInstance::GetMyChar() {
	if (MyChar == nullptr) {
		auto x = TryGetPawnOwner();
		MyChar = Cast<AShootGameCharacter>(x);
	}
	return MyChar;
}
