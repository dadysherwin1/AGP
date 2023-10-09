// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PickupManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class AGP_API UPickupManagerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override
	{
		return TStatId();
	}

protected:
	TArray<FVector> PossibleSpawnLocations;
	float PickupSpawnRate = 5.0f;
	float TimeSinceLastSpawn = 0.0f;

private:
	void PopulateSpawnLocations();
	void SpawnWeaponPickup();
};