// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "TableManager.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

class ATableManager;

UCLASS()
class PINBALL_API ABumper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
		UStaticMeshComponent* BumperStaticMesh;

	// Reference to current TableManager.
	UPROPERTY()
	ATableManager* CurrentGameMode;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly)
		int ScoreValue;

	// Set in constructor. Grabs material assigned in Editor.
	UPROPERTY()
	UMaterialInterface* DefaultMaterial;

	bool bIsPulse = false;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	int ActivePlayerNumber;

	// Reset Bumper's active players and materials
	void ResetBumper();
	

private:

	// Used for pulsing the bumper on hit
	/**
	 * @brief Quickly raises and lowers ABumper's static mesh for visual effect.
	 * @note Increases impulse applied to colliding ball.
	 */
	void PulseBumper(const float Time);
	bool bIsMaxPulseSize = false;
	FVector ExpandedSize; // Set in BeginPlay() adds .5 to the original scale. My opinion this looks best.
	FVector OriginalSize; 
	FVector NewSize;


	UPROPERTY(EditAnywhere)
	bool bAddToBumperCollection;

	// Multi Ball trigger collection
	UPROPERTY(EditAnywhere)
	bool bAddToMultiBallCollection;
	
	
};
