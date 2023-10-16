// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationNode.h"

// Sets default values
ANavigationNode::ANavigationNode()
{
	bNetLoadOnClient = false;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LocationComponent = CreateDefaultSubobject<USceneComponent>("LocationComponent");
	SetRootComponent(LocationComponent);
}

bool ANavigationNode::ShouldTickIfViewportsOnly() const
{
	return true;
}

TArray<ANavigationNode*> ANavigationNode::GetConnectedNodes()
{
	return ConnectedNodes;
}

float ANavigationNode::GetDistanceTo(ANavigationNode* OtherNode)
{
	return FVector::Distance(GetActorLocation(), OtherNode->GetActorLocation());
}

void ANavigationNode::ConnectToNode(ANavigationNode* OtherNode)
{
	const float Rise = GetActorLocation().Z - OtherNode->GetActorLocation().Z;
	const float Run = FVector::Dist2D(GetActorLocation(), OtherNode->GetActorLocation());
	if (Rise/Run < -.9)
		ConnectedNodes.Add(OtherNode);
	else if (Rise/Run > .9)
		OtherNode->ConnectedNodes.Add(this);
	else
	{
		ConnectedNodes.Add(OtherNode);
		OtherNode->ConnectedNodes.Add(this);
	}
}

// Called when the game starts or when spawned
void ANavigationNode::BeginPlay()
{
	Super::BeginPlay();

	// need to ask sir why clients can still see navigation nodes...
	if (GetLocalRole() != ROLE_Authority)
	{
		Destroy();
	}
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

