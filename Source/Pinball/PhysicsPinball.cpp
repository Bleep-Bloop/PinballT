// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsPinball.h"

#include "Paddle.h"
#include "PinballPlayer.h"
#include "Components/SphereComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APhysicsPinball::APhysicsPinball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; // ToDo: False and remove Tick()

	PinBall = CreateDefaultSubobject<USphereComponent>("Sphere Component");
	RootComponent = PinBall;
	//ConfigureCollision();
	
	BallStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Visual Representation");
	BallStaticMesh->SetupAttachment(RootComponent);
	
}

int APhysicsPinball::GetActivePlayerNumber()
{
	return ActivePlayerNumber;
}

// Called when the game starts or when spawned
void APhysicsPinball::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = Cast<ATableManager>(UGameplayStatics::GetGameMode(GetWorld())); 
	
	PinBall->OnComponentHit.AddDynamic(this, &APhysicsPinball::OnHit);

	// Randomize initial direction of APhysicsBall. 
	MovementVector = FVector(1000, UKismetMathLibrary::RandomFloatInRange(-1800,1800),0); // ToDo: Fix HardCoded value. Currently can only spawn on right side of table.
	
	PinBall->SetPhysicsLinearVelocity(MovementVector, false); // ToDo: play with bAddToCurrent it adds velocity to existing.
	
}

// Called every frame
void APhysicsPinball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhysicsPinball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
	if(PreviousCollisionObject != OtherActor)
	{
		PreviousCollisionObject = OtherActor;

		// If paddle collision set ActiveMaterial & PlayerNumber and assign StaticMesh material
		if(OtherActor->IsA(APaddle::StaticClass())) // ToDo: Rework to check less
		{
			this->CurrentActiveMaterial = Cast<APaddle>(OtherActor)->ActiveMaterial;
			BallStaticMesh->SetMaterial(0, CurrentActiveMaterial);
			ActivePlayerNumber = Cast<APaddle>(OtherActor)->GetPlayerNumber();
		}
		
		FVector NewMovementVector = UKismetMathLibrary::GetReflectionVector(MovementVector, Hit.ImpactNormal);

		// ToDo: Polish numbers
		// apply force for better feeling
		//PinBall->AddImpulse(NewMovementVector * 5000,"None", false);
		PinBall->AddImpulse(NewMovementVector * 0.5f, NAME_None, true); // Note: 10000 is fun, 0.5 feels good with bVelChange = true
		PinBall->AddForce(NewMovementVector * 100, NAME_None, true);
		
		MovementVector = NewMovementVector;
	}
}

void APhysicsPinball::ConfigureCollision() const
{
	// ToDo: Ensure nothing missed
	PinBall->SetSimulatePhysics(true);
	PinBall->SetEnableGravity(false);
	PinBall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APhysicsPinball::Destroyed()
{
	CurrentGameMode->DestroyAndCheckBall();
	Super::Destroyed();
}

void APhysicsPinball::SendScore(const int Score) 
{
	if(CurrentGameMode)
		CurrentGameMode->AddScore(ActivePlayerNumber, Score);
}

