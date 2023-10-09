// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

struct FWeaponStats;
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
	bool HasWeapon() const;
	void EquipWeapon(bool bEquipWeapon, const FWeaponStats& WeaponStats);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquip(bool bEquipGun);

	// week 5: firing
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BulletStartPosition;
	bool Fire(const FVector& FireAtLocation);

	// week 7: pcg 2
	UPROPERTY()
	UWeaponComponent* WeaponComponent = nullptr;
	void Reload();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
