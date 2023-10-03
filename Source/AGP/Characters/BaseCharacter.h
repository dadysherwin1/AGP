// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AGP/Pickups/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UWeaponComponent;
class UHealthComponent;

UCLASS()
class AGP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	bool HasWeapon();
	void EquipWeapon(bool bEquipWeapon);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquip(bool bEquipGun);

	// week 5: firing
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BulletStartPosition;
	void Fire(const FVector& FireAtLocation);

	// week 7: pcg 2
	UPROPERTY()
	UWeaponComponent* WeaponComponent = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void EquipWeaponImplementation(bool bEquipWeapon,
	const FWeaponStats& WeaponStats = FWeaponStats());
	UFUNCTION(NetMulticast, Reliable)
	
	void MulticastEquipWeapon(bool bEquipWeapon,
	const FWeaponStats& WeaponStats = FWeaponStats());
	
};
