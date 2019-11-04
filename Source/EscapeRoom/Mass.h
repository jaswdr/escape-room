// Copyright Codeep IT Solutions (c) 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mass.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEROOM_API UMass : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMass();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	float TotalMass = 0.f;
};
