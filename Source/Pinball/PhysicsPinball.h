// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TableManager.h"
#include "GameFramework/Actor.h"
#include "PhysicsPinball.generated.h"

class USphereComponent; 
UCLASS()
class PINBALL_API APhysicsPinball : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsPinball();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Takes a value and send's it to the ActivePlayer's score inside the TableManager.
	 * @param Score Int value added to ActivePlayer's score.
	 */
	void SendScore(int Score);

	UPROPERTY()
	UMaterial* CurrentActiveMaterial;

	int GetActivePlayerNumber();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// ToDo - Remove - Unnecessary
	UPROPERTY(EditAnywhere, Category="RunTime")
		float MovementSpeed = 1000.0f; 
	
	/**
	 * @brief Called when the object collides with another. Used to set new MovementVector based on reflection.
	 * Note - Score is handled from specific collision objects.
	 * @param HitComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param NormalImpulse 
	 * @param Hit 
	 */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:

	UPROPERTY(EditAnywhere, Category="Components")
		USphereComponent* PinBall;

	UPROPERTY(EditAnywhere, Category="Components")
		UStaticMeshComponent* BallStaticMesh;

	// Reference to current TableManager.
	UPROPERTY()
	ATableManager* CurrentGameMode;

	/**
	 * @brief The objects current movement.
	 * @requires float MovementSpeed
	 * @note Set randomly inside BeginPlay()
	 */
	UPROPERTY() 
	FVector MovementVector = FVector(0.0f, -1.0f, 0.0f) * MovementSpeed; 
	
	// Used to ensure collision will not occur multiple times on the same object.
	UPROPERTY()
	AActor* PreviousCollisionObject = nullptr;

	// Last player number the ball had contact with.
	UPROPERTY(VisibleAnywhere, Category="Runtime")
	int ActivePlayerNumber;

	/**
	 * ToDo: Implement
	 * @brief Sets collision settings currently set in derived BP.
	 */
	void ConfigureCollision() const; 

	virtual void Destroyed() override;
	
};
