// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "AGP/PlayerCharacter.h"

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
