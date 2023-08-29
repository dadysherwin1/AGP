// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNode.h"

// Sets default values
ANavigationNode::ANavigationNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LocationComponent = CreateDefaultSubobject<USceneComponent>("LocationComponent");
	SetRootComponent(LocationComponent);
}

bool ANavigationNode::ShouldTickIfViewportsOnly() const
{
	return true;
}

// Called when the game starts or when spawned
void ANavigationNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (LocationComponent)
		DrawDebugSphere(GetWorld(), LocationComponent->GetComponentLocation(), 30, 4, FColor::Cyan);

	for (ANavigationNode* ConnectedNode : ConnectedNodes)
	{
		if (!ConnectedNode) continue;

		FColor Color = FColor::Blue;
		float Thickness = 2;
		if (ConnectedNode==this)
		{
			Color = FColor::Red;
			Thickness = 10;
		}
		else if (ConnectedNode->ConnectedNodes.Contains(this))
			Color = FColor::Green;
		DrawDebugLine(GetWorld(), LocationComponent->GetComponentLocation(), ConnectedNode->LocationComponent->GetComponentLocation(), Color, false, -1, 0, Thickness);
	}
	
}

