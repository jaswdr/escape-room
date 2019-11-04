// Copyright Codeep IT Solutions (c) 2019


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("No PressurePlate attached to %s"), *Owner->GetName())
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float TotalMass = GetTotalMassOfActorsOnPlate();

	// Poll the Trigger Volume
	if (TotalMass == MassRequiredToOpen) { // TODO make into a parameter
		// If the ActorThatOpens is in the volume
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all the overlapping actors
	// Iterate through then adding their masses
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return 0.f; }
	PressurePlate->GetOverlappingActors(
		OUT OverlappingActors
	);

	for (const AActor* Actor:OverlappingActors)
	{
		UPrimitiveComponent* PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();
		TotalMass += PrimitiveComponent->GetMass();
	}

	return TotalMass;
}

