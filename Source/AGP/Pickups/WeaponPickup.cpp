// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPickup.h"

#include "AGP/Characters/PlayerCharacter.h"
#include "Net/UnrealNetwork.h"

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		GenerateWeaponPickup();
	}
	UpdateWeaponPickupMaterial();
}

void AWeaponPickup::OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority) return;
	UE_LOG(LogTemp, Display, TEXT("Overlap event occured in WeaponPickup"));
	ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
	if (Player)
	{
		// if (Player->HasWeapon()) return;
		
		Player->EquipWeapon(true, WeaponStats);
		this->Destroy();
	}
}

void AWeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponPickup, WeaponRarity);
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
	// set rarity
	WeaponRarity = GenerateRarity();

	// pick good stats
	TArray<bool> GoodStats = {false, false, false, false, false};
	switch (WeaponRarity)
	{
	case EWeaponRarity::Legendary:
		PickRandomStat(GoodStats);
	case EWeaponRarity::Master:
		PickRandomStat(GoodStats);
	case EWeaponRarity::Rare:
		PickRandomStat(GoodStats);
	case EWeaponRarity::Common:
		PickRandomStat(GoodStats);
	}

	// set stats
	WeaponStats.Accuracy = GenerateAccuracy(GoodStats[0]);
	WeaponStats.BaseDamage = GenerateBaseDamage(GoodStats[1]);
	WeaponStats.FireRate = GenerateFireRate(GoodStats[2]);
	WeaponStats.MagazineSize = GenerateMagazineSize(GoodStats[3]);
}

float AWeaponPickup::GenerateAccuracy(const bool& bIsGood)
{
	if (bIsGood)
		return FMath::RandRange(GoodAccuracy.X, GoodAccuracy.Y);
	return FMath::RandRange(BadAccuracy.X, BadAccuracy.Y);
}

float AWeaponPickup::GenerateFireRate(const bool& bIsGood)
{
	if (bIsGood)
		return FMath::RandRange(GoodFireRate.X, GoodFireRate.Y);
	return FMath::RandRange(BadFireRate.X, BadFireRate.Y);
}

float AWeaponPickup::GenerateBaseDamage(const bool& bIsGood)
{
	if (bIsGood)
		return FMath::RandRange(GoodBaseDamage.X, GoodBaseDamage.Y);
	return FMath::RandRange(BadBaseDamage.X, BadBaseDamage.Y);
}

uint32 AWeaponPickup::GenerateMagazineSize(const bool& bIsGood)
{
	if (bIsGood)
		return FMath::RandRange(GoodMagazineSize.X, GoodMagazineSize.Y);
	return FMath::RandRange(BadMagazineSize.X, BadMagazineSize.Y-1);
}

float AWeaponPickup::GenerateReloadTime(const bool& bIsGood)
{
	if (bIsGood)
		return FMath::RandRange(GoodReloadTime.X, GoodReloadTime.Y);
	return FMath::RandRange(BadReloadTime.X, BadReloadTime.Y);
}

void AWeaponPickup::PickRandomStat(TArray<bool>& GoodStats)
{
	int Index = FMath::RandRange(0,4);
	while (GoodStats[Index])
		Index = FMath::RandRange(0,4);
	GoodStats[Index] = true;
}
