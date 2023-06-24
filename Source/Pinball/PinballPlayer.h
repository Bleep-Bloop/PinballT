// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PinballPlayer.generated.h"

/**
 * Holds unique information for a player
 * (PlayerNumber, ActiveMaterial)
 */
UCLASS(Blueprintable)
class PINBALL_API UPinballPlayer : public UObject
{
	GENERATED_BODY()
public:
	
	int GetPlayerNumber() const; // ToDo: Double check. I do not believe this is used anywhere.
	
	UMaterial* GetMaterial() const;

private:
	/**
	 * @brief Player 1 or 2.
	 * @note Remember to set in derived PlayerBP
	 */
	UPROPERTY(EditAnywhere)
	int PlayerNumber;

	/**
	 * @brief Paddle material passed to components activated by player.
	 */
	UPROPERTY(EditAnywhere)
	UMaterial* ActiveMaterial;
	
};
