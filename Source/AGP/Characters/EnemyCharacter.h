// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class UPawnSensingComponent;
class UPathfindingSubsystem;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Patrol,
	Engage,
	Evade
};

UCLASS()
class AGP_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY();
	UPathfindingSubsystem* PathfindingSubsystem;
	UPROPERTY(VisibleAnywhere);
	TArray<FVector> CurrentPath;
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	UPROPERTY()
	APlayerCharacter* SensedCharacter = nullptr;
	
	UPROPERTY(EditAnywhere)
	EEnemyState CurrentState = EEnemyState::Patrol;
	void TickPatrol();
	void TickEngage();
	void TickEvade();
	UFUNCTION()
	void OnSensedPawn(APawn* Pawn);
	void UpdateSight();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveAlongPath();

};
