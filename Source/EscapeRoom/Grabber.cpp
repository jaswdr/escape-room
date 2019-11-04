// Copyright Codeep IT Solutions (c) 2019

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
	// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent attached to %s"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent() {
	// Look for attached Input Component (only appear at run time)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent attached to %s"), *GetOwner()->GetName())
		return;
	}

	// Physics handle is found
	UE_LOG(LogTemp, Warning, TEXT("Input Component found!"))

	// Bind the input axis
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed!"))

	/// LINE TRACE and reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	/// If we hit somethiung then attach a physics handle
	if (ActorHit) {
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab release"))

	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto LineTraceEnd = GetReachLineEnd();
	DrawDebugLine(
		GetWorld(),
		GetReachLineStart(),
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) {
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FVector UGrabber::GetReachLineStart()
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Setup  query parameters
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner()); // if not ignore ourself, the first hit will be ourselves

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// Log what we hit
	AActor* HitActor = Hit.GetActor();
	if (HitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitActor->GetName());
	}

	return Hit;
}
