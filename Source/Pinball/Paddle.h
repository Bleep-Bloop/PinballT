// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballPlayer.h"
//#include "TableManager.h"
#include "GameFramework/Actor.h"
#include "Paddle.generated.h"

class ATableManager;

UCLASS()
class PINBALL_API APaddle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetPlayerNumber() const; 
	
	UPROPERTY(EditAnywhere)
	UMaterial* ActiveMaterial;

	/**
	* @brief Takes ActiveMaterial from TableManager's players and sets paddle's materials.
	*/
	void SetActiveMaterial();
	

	UPROPERTY()
	TSubclassOf<UPinballPlayer> OwningPlayer; 

protected:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PaddleStaticMesh;

	UPROPERTY(EditAnywhere)
		float RotateAmount = 55.0f;

	FRotator StartingRotation;
	
	UFUNCTION(BlueprintCallable)
		void ActivatePaddle();

	UFUNCTION(BlueprintCallable)
		void DeactivatePaddle();

	FLatentActionInfo LatentInfo; // QUESTION: What even is this?

	UPROPERTY(EditAnywhere, Category="Config")
	int PlayerNumber = 1;

	// ToDo: Remove - Unnecessary
	UPROPERTY(EditAnywhere, Category="Config")
	bool bIsRightPaddle = true;

	UPROPERTY(EditAnywhere)
	ATableManager* CurrentGameMode;

	/**
	* @brief Checks paddles bIsRightPaddle & PlayerNumber and binds them to inputs.
	*/
	void BindInput();
	

	
};
