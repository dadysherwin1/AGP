// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "AGP/Characters/PlayerCharacter.h"

void AWeaponPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Overlap event occured in WeaponPickup"));
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		if (Player->HasWeapon()) return;
		
		Player->EquipWeapon(true);
		this->Destroy();
	}
}

EWeaponRarity AWeaponPickup::GenerateRarity()
{
	const float RandNum = FMath::RandRange(0.f,1.f);
	if (RandNum <= .05f)
		return EWeaponRarity::Legendary;
	if (RandNum <= .2f)
		return EWeaponRarity::Master;
	if (RandNum <= .5f)
		return EWeaponRarity::Rare;
	return EWeaponRarity::Common;
}

void AWeaponPickup::GenerateWeaponPickup()
{
	// EWeaponRarity WeaponRarity = GenerateRarity();
	
}
