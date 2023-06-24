// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDrainBox.h"

#include "PhysicsPinball.h"
#include "Kismet/GameplayStatics.h"


APlayerDrainBox::APlayerDrainBox()
{
	OnActorBeginOverlap.AddDynamic(this, &APlayerDrainBox::OnOverlapBegin);
}

void APlayerDrainBox::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = Cast<ATableManager>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APlayerDrainBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor->IsA(APhysicsPinball::StaticClass()))
	{

		APhysicsPinball* CurrentBallCollision = Cast<APhysicsPinball>(OtherActor); 
		//if(OwningPlayerNumber != CurrentBallCollision->GetActivePlayerNumber())
		//{
			CurrentGameMode->SubtractScore(OwningPlayerNumber, 20);
			// ToDo: Also reward the other player if it was his active ball.
			
	//	}
		CurrentBallCollision->Destroy();
	}
	
}


