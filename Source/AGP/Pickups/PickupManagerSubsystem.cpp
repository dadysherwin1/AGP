// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "WeaponPickup.h"
#include "AGP/AGPGameInstance.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetNetMode() == NM_Client) return;

	//if (PossibleSpawnLocations.IsEmpty())
	//	PopulateSpawnLocations();

	TimeSinceLastSpawn += DeltaTime;
	if (TimeSinceLastSpawn >= PickupSpawnRate)
	{
		TimeSinceLastSpawn = 0.f;
		SpawnWeaponPickup();
	}
}

void UPickupManagerSubsystem::DestroyWeaponPickup(AWeaponPickup* WeaponPickup)
{
	for (int i = 0; i < SpawnedWeapons.Num(); i++)
	{
		if (SpawnedWeapons[i] == WeaponPickup)
		{
			WeaponPickup->Destroy();
			SpawnedWeapons[i] = nullptr;
			break;
		}
	}
}

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	// PossibleSpawnLocations.Empty();
	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	// checks
	if (GetWorld()->GetNetMode() == NM_Client) return;
	if (PossibleSpawnLocations.IsEmpty()) return;
	const UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>();
	if (!GameInstance) return;
	
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
}

void UPickupManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	if (GetWorld()->GetNetMode() == NM_Client) return;
	
	PopulateSpawnLocations();
	SpawnedWeapons.Init(nullptr, PossibleSpawnLocations.Num());
	
	// crashes...
	// if (GetWorld()->GetNetMode() == NM_Client)
	// 	this->Deinitialize();
}