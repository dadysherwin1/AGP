// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "HealthComponent.h"
#include "AGP/Pickups/WeaponComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BulletStartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletStartPosition"));
	BulletStartPosition->SetupAttachment(RootComponent);
	UHealthComponent* HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseCharacter::Fire(const FVector& FireAtLocation)
{
	if (HasWeapon())
		return WeaponComponent->Fire(GetActorLocation(), FireAtLocation);
	return false;
}

void ABaseCharacter::Reload()
{
	if (!HasWeapon()) return;
	WeaponComponent->ReloadWeapon();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseCharacter::HasWeapon() const
{
	if (WeaponComponent)
		return true;
	return false;
}

void ABaseCharacter::EquipWeapon(bool bEquipWeapon, const FWeaponStats& WeaponStats)
{
	if (bEquipWeapon && !HasWeapon()) // equip gun
	{
		WeaponComponent = NewObject<UWeaponComponent>(this);
		WeaponComponent->SetWeaponStats(WeaponStats);
		WeaponComponent->RegisterComponent();
	}
	else if (!bEquipWeapon && HasWeapon()) // unequip gun
	{
		WeaponComponent->UnregisterComponent();
		WeaponComponent = nullptr;
	}
	else if (bEquipWeapon && HasWeapon()) // unequip and equip gun
	{
		WeaponComponent->UnregisterComponent();
		WeaponComponent = NewObject<UWeaponComponent>(this);
		WeaponComponent->SetWeaponStats(WeaponStats);
		WeaponComponent->RegisterComponent();
	}
	
	OnWeaponEquip(bEquipWeapon);
}

