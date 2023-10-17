// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPGameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Pickups/WeaponPickup.h"

UClass* UAGPGameInstance::GetWeaponPickupClass() const
{
	return WeaponPickupClass.Get();
}

void UAGPGameInstance::SpawnGroundHitParticles(const FVector& SpawnLocation)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundHitParticles, SpawnLocation);
}

void UAGPGameInstance::PlayGunshotSoundAtLocation(const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), GunshotSoundCue, Location, FRotator::ZeroRotator);
}

void UAGPGameInstance::PlayGunshotSound2D()
{
	UGameplayStatics::PlaySound2D(GetWorld(), GunshotSoundCue);
}
