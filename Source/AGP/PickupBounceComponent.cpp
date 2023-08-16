// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBounceComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnitConversion.h"

// Sets default values for this component's properties
UPickupBounceComponent::UPickupBounceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPickupBounceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPickupBounceComponent::TickBounceObject(const float& DeltaTime)
{
	FVector CurrentActorLocation = GetOwner()->GetActorLocation();
	const double CurrentTime = UGameplayStatics::GetTimeSeconds(GetOwner());
	CurrentActorLocation.Z += sin(CurrentTime) * BounceSpeed;
	GetOwner()->SetActorLocation(CurrentActorLocation);
}

// Called every frame
void UPickupBounceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickBounceObject(DeltaTime);
}

