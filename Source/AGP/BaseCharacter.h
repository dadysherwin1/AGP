// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class AGP_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UFUNCTION(BlueprintCallable)
	bool HasWeapon();
	void EquipWeapon(bool bEquipGun);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponEquip(bool bEquipGun);
	bool bHasWeaponEquipped = false;

	// week 5: firing
	float TimeSinceLastShot = 0.0f;
	float MinTimeBetweenShots = 0.2f;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BulletStartPosition;
	bool Fire(const FVector& FireAtLocation);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
