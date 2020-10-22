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


	// ֻ��Ϊ����Ue4�������Զ������ڴ�
	UPROPERTY()
	class UUserWidget* CurrentWidget;
};



