// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootGameGameMode.generated.h"

UCLASS(minimalapi)
class AShootGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShootGameGameMode();


	virtual void BeginPlay() override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShootGame")
	TSubclassOf<class UUserWidget> HUDWidgetClass;


	// 只是为了让Ue4给我们自动管理内存
	UPROPERTY()
	class UUserWidget* CurrentWidget;
};



