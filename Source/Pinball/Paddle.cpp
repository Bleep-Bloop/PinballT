// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"

#include "LatentActions.h"
#include "PinballGameModeBase.h"
#include "TableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APaddle::APaddle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaddleStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Paddle Static Mesh");

	RootComponent = PaddleStaticMesh;

	PaddleStaticMesh->SetNotifyRigidBodyCollision(true);
	PaddleStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // maybe just physics
	PaddleStaticMesh->SetSimulatePhysics(true);
	PaddleStaticMesh->SetEnableGravity(false);
	//Paddle->SetMassOverrideInKg(, 9999.f, true);
	PaddleStaticMesh->GetBodyInstance()->SetMassOverride(9599.f, true);
	
	
	PaddleStaticMesh->GetBodyInstance()->bLockXTranslation = true;
	PaddleStaticMesh->GetBodyInstance()->bLockYTranslation = true;
	PaddleStaticMesh->GetBodyInstance()->bLockZTranslation = true;
	PaddleStaticMesh->GetBodyInstance()->bLockXRotation = true;
	PaddleStaticMesh->GetBodyInstance()->bLockYRotation = true;
	//Paddle->SetConstraintMode(EDOFMode::SixDOF);
	
	LatentInfo.CallbackTarget = this;
	
}


// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameMode = Cast<ATableManager>(UGameplayStatics::GetGameMode(GetWorld())); // Question: why not StaticCast<>()?
	
	// Setup Paddles
	StartingRotation = PaddleStaticMesh->GetComponentRotation();
	CurrentGameMode->AddPaddle(this);
	BindInput();
	SetActiveMaterial();
	
}

// Called every frame
void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int APaddle::GetPlayerNumber() const
{
	return PlayerNumber;
}


void APaddle::BindInput()
{
	if(InputComponent)
	{
		if(PlayerNumber == 1)
		{
			if(bIsRightPaddle == true)
			{
				InputComponent->BindAction("P1_RightPaddle", IE_Pressed, this, &APaddle::ActivatePaddle);
				InputComponent->BindAction("P1_RightPaddle", IE_Released, this, &APaddle::DeactivatePaddle);
			}
			else
			{
				InputComponent->BindAction("P1_LeftPaddle", IE_Pressed, this, &APaddle::ActivatePaddle);
				InputComponent->BindAction("P1_LeftPaddle", IE_Released, this, &APaddle::DeactivatePaddle);
			}
		}
		else
		{
			if(bIsRightPaddle == true)
			{
				InputComponent->BindAction("P2_RightPaddle", IE_Pressed, this, &APaddle::ActivatePaddle);
				InputComponent->BindAction("P2_RightPaddle", IE_Released, this, &APaddle::DeactivatePaddle);
			}
			else
			{
				InputComponent->BindAction("P2_LeftPaddle", IE_Pressed, this, &APaddle::ActivatePaddle);
				InputComponent->BindAction("P2_LeftPaddle", IE_Released, this, &APaddle::DeactivatePaddle);
			}
		}
	}
	
}

void APaddle::SetActiveMaterial()
{
	if(PlayerNumber == 1)
	{
		OwningPlayer = CurrentGameMode->Player1;
		UPinballPlayer* PlayerGrab = Cast<UPinballPlayer>(CurrentGameMode->Player1.GetDefaultObject()); // these three lines make it work perfectly
		ActiveMaterial = Cast<UPinballPlayer>(PlayerGrab)->GetMaterial();
	}
	else if(PlayerNumber == 2)
	{
		OwningPlayer = CurrentGameMode->Player2;
		UPinballPlayer* PlayerGrab = Cast<UPinballPlayer>(CurrentGameMode->Player2.GetDefaultObject()); // these three lines make it work perfectly
		ActiveMaterial = Cast<UPinballPlayer>(PlayerGrab)->GetMaterial();
	}

	// ToDo: Set a small circle on the pivot the color. Will look nicer.
	if(ActiveMaterial)
	{
		PaddleStaticMesh->SetMaterial(0, ActiveMaterial);
	}
	
}

void APaddle::ActivatePaddle()
{
	const FRotator ActivePosition = FRotator(0, (StartingRotation.Yaw + RotateAmount), 0);
	
	UKismetSystemLibrary::MoveComponentTo(PaddleStaticMesh, GetTransform().GetLocation(), ActivePosition, true, true, .25f, true, EMoveComponentAction::Move, LatentInfo);
	
}

void APaddle::DeactivatePaddle()
{
	UKismetSystemLibrary::MoveComponentTo(PaddleStaticMesh, GetTransform().GetLocation(), StartingRotation, true, true, .25f, true, EMoveComponentAction::Move, LatentInfo);
}

