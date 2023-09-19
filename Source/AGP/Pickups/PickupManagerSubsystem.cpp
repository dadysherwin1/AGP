// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManagerSubsystem.h"

#include "AGP/Pathfinding/PathfindingSubsystem.h"

void UPickupManagerSubsystem::PopulateSpawnLocations()
{
	PossibleSpawnLocations.Empty();

	PossibleSpawnLocations = GetWorld()->GetSubsystem<UPathfindingSubsystem>()->GetWaypointPositions();
}

void UPickupManagerSubsystem::SpawnWeaponPickup()
{
	
}
