// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "Pathfinding/PathfindingSubsystem.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	PathfindingSubsystem = GetWorld()->GetSubsystem<UPathfindingSubsystem>();
	CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (FVector::Distance(GetActorLocation(), CurrentPath.Last()) <= 150)
	{
		CurrentPath.Pop();
		if (CurrentPath.Num() == 0)
			CurrentPath = PathfindingSubsystem->GetRandomPath(GetActorLocation());;
	}

	const FVector Direction = CurrentPath.Last() - GetActorLocation();
	AddMovementInput(Direction);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

