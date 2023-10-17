// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AGPGameInstance.generated.h"

class USoundCue;
class AWeaponPickup;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class AGP_API UAGPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UClass* GetWeaponPickupClass() const;

	// week 10: effects
	void SpawnGroundHitParticles(const FVector& SpawnLocation);
	void PlayGunshotSoundAtLocation(const FVector& Location);
	void PlayGunshotSound2D();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Pickup Classes")
	TSubclassOf<AWeaponPickup> WeaponPickupClass;

	// week 10: effects
	UPROPERTY(EditDefaultsOnly, Category="Particle Systems")
	UNiagaraSystem* GroundHitParticles;
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundCue* GunshotSoundCue;
};
