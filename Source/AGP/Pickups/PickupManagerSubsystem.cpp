// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (PossibleSpawnLocations.IsEmpty())
	//	PopulateSpawnLocations();

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		TimeSinceLastSpawn -= PickupSpawnRate;
		SpawnWeaponPickup();
	}
}

void UPickupManagerSubsystem::DestroyWeaponPickup(AWeaponPickup* WeaponPickup)
{
	if (const uint16 Index = SpawnedWeapons.Find(WeaponPickup) != INDEX_NONE)
	{
		WeaponPickup->Destroy();
		SpawnedWeapons[Index] = nullptr;
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	// PossibleSpawnLocations.Empty();

	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	if (GetWorld()->GetNetMode() == NM_Client)
		return;
	
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
	const int NodeIndex = FMath::RandRange(0, PossibleSpawnLocations.Num()-1);
	FVector SpawnPosition = PossibleSpawnLocations[NodeIndex];
	SpawnPosition.Z += 50.0f;

	// remove existing pickup if necessary
	if (AWeaponPickup* PreviousPickup = SpawnedWeapons[NodeIndex])
		DestroyWeaponPickup(PreviousPickup);

	// spawn weapon blueprint
	AWeaponPickup* Pickup = GetWorld()->SpawnActor<AWeaponPickup>(
		GameInstance->GetWeaponPickupClass(), SpawnPosition, FRotator::ZeroRotator);
	SpawnedWeapons[NodeIndex] = Pickup;
	UE_LOG(LogTemp, Display, TEXT("Weapon Pickup Spawned"));
	
}

void UPickupManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	PopulateSpawnLocations();
	UE_LOG(LogTemp, Log, TEXT("%i"), PossibleSpawnLocations.Num());
	SpawnedWeapons.Init(nullptr, PossibleSpawnLocations.Num());
	
	// crashes...
	// if (GetWorld()->GetNetMode() == NM_Client)
	// 	this->Deinitialize();
}