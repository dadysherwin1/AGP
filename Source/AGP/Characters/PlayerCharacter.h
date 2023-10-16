// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPlayerCharacterHUD;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class AGP_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void OnDeath() override;
	void BeginPlayImplementation();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowBodyMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly) UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly) float LookSensitivity;
	UPROPERTY(EditDefaultsOnly) UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly) UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly) UInputAction* ReloadAction;
	
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* InputMappingContext;

	// week 9: multiplayer part 2
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerCharacterHUD> PlayerHUDClass;
	UPROPERTY()
	UPlayerCharacterHUD* PlayerHUD;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// week 9: multiplayer part 2
	void UpdateHealthBar(float HealthPercent);
	void UpdateAmmoText(int Ammo);
	void UpdateMagazineSizeText(int MagazineSize);

	
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MyJump(const FInputActionValue& Value);
	void FireWeapon(const FInputActionValue& Value);
};
