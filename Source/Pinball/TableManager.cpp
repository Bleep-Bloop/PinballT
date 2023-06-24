// Fill out your copyright notice in the Description page of Project Settings.


#include "TableManager.h"

#include <string>

#include "PhysicsPinball.h"
#include "Kismet/GameplayStatics.h"

void ATableManager::AddPaddle(APaddle* Paddle)
{
	TablePaddles.Add(Paddle);
}

void ATableManager::BeginPlay()
{
	Super::BeginPlay();

	// Initial Game Ball - With Delay
	GetWorldTimerManager().SetTimer(RoundStartSpawnTimerHandle, this, &ATableManager::CreateBall, 4, false);

	// Debug Multi Ball Start - With Delay
	if(DEBUG_MULTIBALLSTART)
		GetWorldTimerManager().SetTimer(RoundStartMultiBallSpawnTimerHandle, this, &ATableManager::StartMultiBall, 5, false);
	
}

void ATableManager::AddScore(const int PlayerNumber, int const Score)
{
	if(PlayerNumber == 1)
		Player1Score += Score;
	else
		Player2Score += Score;

	UpdateUI();
}

void ATableManager::SubtractScore(const int PlayerNumber, const int Score)
{
	if(PlayerNumber == 1)
	{
		Player1Score -= Score;
		if(Player1Score < 0)
			Player1Score = 0;
	}
	else
	{
		Player2Score -= Score;
		if(Player2Score < 0)
			Player2Score = 0;
	}

	UpdateUI();
}

void ATableManager::AddToBumperCollection(ABumper* Bumper)
{
	BumperCollection.Add(Bumper);
}

void ATableManager::CheckBumperCollection(const int PlayerNumber)
{

	// ToDo: Crash if empty put check

	int ActiveCount = 0;
	const int BumperCollectionLength = BumperCollection.Num();
	
	for (ABumper* Bumper : BumperCollection)
	{
		if(Bumper->ActivePlayerNumber == PlayerNumber)
		{
			ActiveCount++;
		}
	}

	if(ActiveCount == BumperCollectionLength)
	{
		AddScore(PlayerNumber, CollectionScoreValue);
		for (ABumper* Bumper : BumperCollection)
		{
			Bumper->ResetBumper();
		}
	}
}

void ATableManager::AddToMultiBalLTriggerCollection(ABumper* Bumper)
{
	MultiBallTriggerCollection.Add(Bumper);
}

void ATableManager::CheckMultiBallTriggerCollection(const int PlayerNumber)
{
	
	int ActiveCount = 0;
	const int MultiBallTriggerCollectionLength = MultiBallTriggerCollection.Num();
	
	for (ABumper* MultiTrigger : MultiBallTriggerCollection)
	{
		if(MultiTrigger->ActivePlayerNumber == PlayerNumber)
		{
			ActiveCount++;
		}
	}

	if(ActiveCount == MultiBallTriggerCollectionLength)
	{
		AddScore(PlayerNumber, CollectionScoreValue);
		StartMultiBall();
		for (ABumper* MultiTrigger : MultiBallTriggerCollection)
		{
			MultiTrigger->ResetBumper();
		}
	}
}

void ATableManager::StartMultiBall()
{
	GetWorldTimerManager().SetTimer(StartMultiBallSpawnTimerHandle, this, &ATableManager::CreateBall, 1, true, 0);
	GetWorldTimerManager().SetTimer(StopMultiBallSpawnTimerHandle, this, &ATableManager::StopMultiBall, 5, false);
}

void ATableManager::StopMultiBall()
{
	GetWorldTimerManager().ClearTimer(StartMultiBallSpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(StopMultiBallSpawnTimerHandle);
}

void ATableManager::DestroyAndCheckBall()
{
	BallCount--;
	if(BallCount == 0)
	{
		if(Player1Score > Player2Score)
			UGameplayStatics::OpenLevel(this->GetWorld(), "Map_P1Win");
		else
			UGameplayStatics::OpenLevel(this->GetWorld(), "Map_P2Win");
	}
}

// Note: Direction of the ball is set in PhysicsPinball BeginPlay().
void ATableManager::CreateBall()
{
	// ToDo: Get location of BallSpawner. Currently (-1540.0, 1230.0, -120.0) 
	GetWorld()->SpawnActor<APhysicsPinball>(GameBall, BallSpawnLocation, FRotator(0,0,0));
	BallCount++;
}




