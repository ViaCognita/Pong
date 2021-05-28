// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bound.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class USoundBase;

UCLASS()
class POMG_API ABound : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABound();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// Sound played when the ball hits this Paddle.
	USoundBase* HitSound;

};
