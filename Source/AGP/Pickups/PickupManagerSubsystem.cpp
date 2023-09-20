// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PossibleSpawnLocations.IsEmpty())
		PopulateSpawnLocations();

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		TimeSinceLastSpawn -= PickupSpawnRate;
		SpawnWeaponPickup();
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	// PossibleSpawnLocations.Empty();

	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	// check if theres nodes to spawn on
	if (PossibleSpawnLocations.IsEmpty())
	{
		
		UE_LOG(LogTemp, Error, TEXT("Unable to spawn weapon pickup."));
		return;
	}
	
	const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>();

	// check for nullptr
	if (!GameInstance)
		return;

	// get spawn position
	FVector SpawnPosition = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num()-1)];
	SpawnPosition.Z += 50.0f;

	// spawn weapon blueprint
	AWeaponPickup* Pickup =GetWorld()->SpawnActor<AWeaponPickup>(
		GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);
	UE_LOG(LogTemp, Display, TEXT("Weapon Pickup Spawned"));
}
