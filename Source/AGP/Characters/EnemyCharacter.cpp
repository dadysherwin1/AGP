// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "AGP/Pathfinding/PathfindingSubsystem.h"
#include "AGP/Pickups/WeaponComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() < 3) return;
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSensedPawn);
}

void AEnemyCharacter::TickPatrol()
{
	if (CurrentPath.Num() == 0)
		CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
	MoveAlongPath();
}

void AEnemyCharacter::TickEngage()
{
	if (!SensedCharacter)
		return;
	if (CurrentPath.Num() == 0)
		CurrentPath = PathfindingSubsystem->GetPath(GetActorLocation(),
			SensedCharacter->GetActorLocation());
	MoveAlongPath();
	Fire(SensedCharacter->GetActorLocation());
	if (HasWeapon() && WeaponComponent->IsWeaponEmpty())
		Reload();
}

void AEnemyCharacter::TickEvade()
{
	if (!SensedCharacter) return;
	if (CurrentPath.Num() == 0)
		CurrentPath = PathfindingSubsystem->GetPathAway(GetActorLocation(),
			SensedCharacter->GetActorLocation());
	MoveAlongPath();
}

void AEnemyCharacter::OnSensedPawn(APawn* Pawn)
{
	if (APlayerCharacter* Character = Cast<APlayerCharacter>(Pawn))
	{
		SensedCharacter = Character;
		UE_LOG(LogTemp, Log, TEXT("Sensed Player"));
	}
}

void AEnemyCharacter::UpdateSight()
{
	if (!SensedCharacter)
		return;

	if (!PawnSensingComponent->HasLineOfSightTo(SensedCharacter))
	{
		UE_LOG(LogTemp, Log, TEXT("Lost Player"));
		SensedCharacter = nullptr;
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	if (GetLocalRole() < 3) return;
	
	Super::Tick(DeltaTime);
	UpdateSight();
	
	if (CurrentState == EEnemyState::Patrol)
	{
		if (SensedCharacter)
		{
			CurrentPath.Empty();
			if (GetComponentByClass<UHealthComponent>()->GetCurrentHealthPercentage() >= .4f)
			{
				CurrentState = EEnemyState::Engage;
			}
			else {
				CurrentState = EEnemyState::Evade;
			}
		}
	}	
	else if (CurrentState == EEnemyState::Engage)
	{
		if (SensedCharacter == nullptr)
		{
			CurrentState = EEnemyState::Patrol;
		}
		else if (GetComponentByClass<UHealthComponent>()->GetCurrentHealthPercentage() < .4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Evade;
		}
	}	
	else
	{
		if (!SensedCharacter)
		{
			CurrentState = EEnemyState::Patrol;
		}
		else if (GetComponentByClass<UHealthComponent>()->GetCurrentHealthPercentage() >= .4f)
		{
			CurrentPath.Empty();
			CurrentState = EEnemyState::Engage;
		}
	}
		
	
	switch (CurrentState)
	{
	case EEnemyState::Patrol:
		TickPatrol();
		break;
	case EEnemyState::Engage:
		TickEngage();
		break;
	case EEnemyState::Evade:
		TickEvade();
		break;
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::MoveAlongPath()
{
	if (CurrentPath.IsEmpty()) return;
	if (FVector::Distance(GetActorLocation(), CurrentPath.Last()) <= 150)
	{
		CurrentPath.Pop();
	}
	else
	{
		const FVector Direction = CurrentPath.Last() - GetActorLocation();
		AddMovementInput(Direction);
	}
}

