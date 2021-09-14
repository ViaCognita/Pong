// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class USoundBase;

UCLASS()
class POMG_API AGoal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGoal();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

public:

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	// Sound played when the ball overlap this actor.
	USoundBase* HitSound;

};
