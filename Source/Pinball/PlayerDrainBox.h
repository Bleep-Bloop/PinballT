// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableManager.h"
#include "Engine/TriggerBox.h"
#include "PlayerDrainBox.generated.h"

/**
 * 
 */
UCLASS()
class PINBALL_API APlayerDrainBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	APlayerDrainBox();
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int OwningPlayerNumber;

private:

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	UPROPERTY(EditAnywhere)
	ATableManager* CurrentGameMode;
	
};
