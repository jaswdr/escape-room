// Copyright Codeep IT Solutions (c) 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// How far ahead of  the player can we reach in cm
	float Reach = 150.f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	/// Ray-cast and grab what's in reach
	void Grab();

	/// Called when grab is released.
	void Release();

	/// Find (assumed) attached physic handle component
	void FindPhysicsHandleComponent();

	/// Setup (assumed) attached input component
	void SetupInputComponent();

	/// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
public:	
	/// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Return current end of reach line
	FVector GetReachLineStart();

	// Return current end of reach line
	FVector GetReachLineEnd();
};
