// Fill out your copyright notice in the Description page of Project Settings.


#include "Bumper.h"

#include "PhysicsPinball.h"
#include "VectorTypes.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABumper::ABumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BumperStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("BumperMesh");
	RootComponent = BumperStaticMesh;
	
}

// Called when the game starts or when spawned
void ABumper::BeginPlay()
{
	Super::BeginPlay();

	DefaultMaterial = BumperStaticMesh->GetMaterial(0);
	
	
	CurrentGameMode = Cast<ATableManager>(UGameplayStatics::GetGameMode(GetWorld()));

	if(bAddToBumperCollection)
		CurrentGameMode->AddToBumperCollection(this);
	else if (bAddToMultiBallCollection)
	{
		CurrentGameMode->AddToMultiBalLTriggerCollection(this);
	}

	
	BumperStaticMesh->OnComponentHit.AddDynamic(this, &ABumper::OnHit);

	OriginalSize = BumperStaticMesh->GetRelativeScale3D();
	NewSize = OriginalSize;
	ExpandedSize = OriginalSize + .5;
	
}

void ABumper::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(OtherActor->IsA(APhysicsPinball::StaticClass()))
	{
		if(APhysicsPinball* CurrentBallCollision = Cast<APhysicsPinball>(OtherActor) )
		{
			ActivePlayerNumber = CurrentBallCollision->GetActivePlayerNumber();
			if(ActivePlayerNumber == 1 || ActivePlayerNumber == 2)
			{
				if(bAddToBumperCollection)
				{
					CurrentGameMode->CheckBumperCollection(ActivePlayerNumber);
				}
				else if (bAddToMultiBallCollection)
				{
					CurrentGameMode->CheckMultiBallTriggerCollection(ActivePlayerNumber);
				}
				BumperStaticMesh->SetMaterial(0, CurrentBallCollision->CurrentActiveMaterial);
				CurrentBallCollision->SendScore(ScoreValue);
				
				bIsPulse = true;
			}
		}
	}
}

// Called every frame
void ABumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(bIsPulse)
		PulseBumper(DeltaTime); // Called in Tick because RIterpTo used need DeltaTime
	
}

void ABumper::ResetBumper()
{
	BumperStaticMesh->SetMaterial(0,DefaultMaterial);
	ActivePlayerNumber = 0;
}

void ABumper::PulseBumper(const float Time)
{
	if(bIsMaxPulseSize == false)
	{
		NewSize = FMath::VInterpTo(BumperStaticMesh->GetRelativeScale3D(), ExpandedSize, Time, 30);
		BumperStaticMesh->SetRelativeScale3D(NewSize);

		if(BumperStaticMesh->GetRelativeScale3D() == ExpandedSize)
			bIsMaxPulseSize = true;
	}
	else
	{
		NewSize = FMath::VInterpTo(BumperStaticMesh->GetRelativeScale3D(), OriginalSize, Time, 30); 
		BumperStaticMesh->SetRelativeScale3D(NewSize);
		if(BumperStaticMesh->GetRelativeScale3D() == OriginalSize)
		{
			bIsMaxPulseSize = false;
			bIsPulse = false;
		}
	}

}

