// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "../MyHUD.h"

void AMyGameHUD::BeginPlay() { 

	Super::BeginPlay();

	if (MyWidgetClass != nullptr) {
		auto MyWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(),MyWidgetClass);
		if (MyWidget) {
			MyGameUW = Cast<UMyHUD>(MyWidget);
			if (MyGameUW) { 
				MyGameUW->AddToViewport();
			}
		}
	}
}