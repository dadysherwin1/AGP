// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CoreFwd.h"
#include "AGP/UI/PlayerCharacterHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerCharacter::OnDeath()
{
	if (PlayerHUD)
		PlayerHUD->RemoveFromParent();
	Super::OnDeath();
}

void APlayerCharacter::BeginPlayImplementation()
{
	// Input system
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// HUD
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		PlayerHUD = CreateWidget<UPlayerCharacterHUD>(PlayerController, PlayerHUDClass);
		if (PlayerHUD)
		{
			PlayerHUD->AddToPlayerScreen();
		}
	}
	UpdateHealthBar(1.0f);

	ShowBodyMesh();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	BeginPlayImplementation();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// double Pitch = GetController()->GetControlRotation().Pitch;
	// FTransform Transform = FTransform(FVector(0,0,-155));
	// Transform *= FTransform(FRotator(Pitch,0,0));
	// Transform *= FTransform(FVector(0,0,60));
	// GetMesh()->SetRelativeTransform(Transform);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MyJump);
		Input->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::FireWeapon);
		Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Reload);
	}
}

void APlayerCharacter::UpdateHealthBar(float HealthPercent)
{
	if (IsLocallyControlled() && PlayerHUD)
		PlayerHUD->SetHealthBar(HealthPercent);
}

void APlayerCharacter::UpdateAmmoText(int Ammo)
{
	if (IsLocallyControlled() && PlayerHUD)
		PlayerHUD->SetAmmoText(Ammo);
}

void APlayerCharacter::UpdateMagazineSizeText(int MagazineSize)
{
	if (IsLocallyControlled() && PlayerHUD)
		PlayerHUD->SetMagazineSizeText(MagazineSize);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// get input movement direction, in object space
	const FVector2D MovementVector = Value.Get<FVector2D>();

	// get forward and right direction of the character, in world space
	const FVector ForwardVector = GetActorForwardVector();
	const FVector RightVector = GetActorRightVector();
	
	// use above values to move character
	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>() * LookSensitivity;
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APlayerCharacter::MyJump(const FInputActionValue& Value)
{
	Super::Jump();
}

void APlayerCharacter::FireWeapon(const FInputActionValue& Value)
{
	
	FVector Location;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Location, Rotation);
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(Rotation);
	Fire(GetActorLocation() + 10000 * ForwardVector);
}
