// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PickupManagerSubsystem.generated.h"

class AWeaponPickup;
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

	// week 10: polish
	void DestroyWeaponPickup(AWeaponPickup* WeaponPickup);
	
protected:
	TArray<FVector> PossibleSpawnLocations;
	float PickupSpawnRate = 1.0f;
	float TimeSinceLastSpawn = 0.0f;

	// week 10: polish
	UPROPERTY()
	TArray<AWeaponPickup*> SpawnedWeapons;
	
private:
	void PopulateSpawnLocations();
	void SpawnWeaponPickup();

	void OnWorldBeginPlay(UWorld& InWorld) override;
};
