// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bumper.h"
#include "Paddle.h"
//#include "PhysicsPinball.h"
#include "PinballPlayer.h"
#include "GameFramework/GameModeBase.h"
#include "TableManager.generated.h"

class APhysicsPinball;

/**
 * Handles the general runtime of aPinball2 table. Holds references to players and table components.
 * Deals with creating balls and win/loss.
 */
UCLASS()
class PINBALL_API ATableManager : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere) 
	TSubclassOf<UPinballPlayer> Player1;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPinballPlayer> Player2;
	
	/** ToDo: Delete - The TArray used is currently not used for anything.
	 * @brief Add an instance of APaddle to the TablePaddles array
	 * @param Paddle Instance of APaddle
	 */
	void AddPaddle(APaddle* Paddle);

	
	// Handle Player Scoring //
	void AddScore(const int PlayerNumber, const int Score);
	void SubtractScore(const int PlayerNumber, const int Score);


	// Handle ABumper Collections //
	void AddToBumperCollection(ABumper* Bumper);

	/**
	 * @brief Checks all Bumpers in TArray to see if they are all activated by given player.
	 * @note Called inside ABumper's OnHit().
	 */
	void CheckBumperCollection(int PlayerNumber); 
	
	void AddToMultiBalLTriggerCollection(ABumper* Bumper);

	/**
	 * @brief Checks all ABumpers in TArray to see if they are activated by the given player.
	 * @note Called inside ABumper's OnHit().
	 */
	void CheckMultiBallTriggerCollection(int PlayerNumber); // Call in OnHit() ABumper. Then it does not need to be continuously checked.
	
protected:
	UPROPERTY(BlueprintReadWrite)
	int Player1Score = 0;

	UPROPERTY(BlueprintReadWrite)
	int Player2Score = 0;

	// Updates UI in BP_TableManager
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUI();

	/**
	 * @brief Holds all bumpers belonging to a MultiBall event.
	 * Once all belonging ABumper instances are active MultiBall event starts.
	 * @note ABumpers are added to respective TArrays in their BeginPlay()
	 */
	UPROPERTY()
	TArray<ABumper*> BumperCollection;

	/**
	 * @brief Holds all bumpers belonging to a MultiBall event.
	 * Once all belonging ABumper instances are active MultiBall event starts.
	 * @note ABumpers are added to respective TArrays in their BeginPlay()
	 */
	UPROPERTY()
	TArray<ABumper*> MultiBallTriggerCollection;

	// Used for handling timed starts on RoundStart and MultiBall event. // Note double check these timers somethings missing
	FTimerHandle RoundStartSpawnTimerHandle;
	FTimerHandle RoundStartMultiBallSpawnTimerHandle;
	FTimerHandle StartMultiBallSpawnTimerHandle;
	FTimerHandle StopMultiBallSpawnTimerHandle;
	
	void StartMultiBall();
	void StopMultiBall();

	/**
	 * @brief Creates an instance of GameBall on TableManager->BallSpawnLocation;
	 */
	void CreateBall(); 

	FVector BallSpawnLocation = FVector(-1540, 1230, 80); // ToDo: Not hardcoded. Fix so easier to create new tables.

	/**
	 * @brief  The BP of the PhysicsPinball used in game.
	 * @note Set in BP_TableManager for each level.
	 */
	UPROPERTY(EditAnywhere) 
	TSubclassOf<APhysicsPinball> GameBall;

	// ToDO: Delete. Currently no use for this.
	UPROPERTY(VisibleAnywhere)
	TArray<APaddle*> TablePaddles;

	/**
	 * @brief Score value for activating all ABumpers in a collection
	 * ToDo: Seperate value MultiBall event
	 */
	UPROPERTY(EditDefaultsOnly)
	int CollectionScoreValue = 100;

public:
	/**
	 * @brief Subtracts from BallCount and checks win condition if no balls left.
	 * @note Called inside destructor for APhysicsPinball
	 */
	void DestroyAndCheckBall();

	/**
	 * @brief Number of APhysicsPinballs in game.
	 */
	UPROPERTY()
	int BallCount = 0;
	
	UPROPERTY(EditAnywhere, Category="DEBUG")
	bool DEBUG_MULTIBALLSTART = false;

	
};
