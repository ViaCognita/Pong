// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class POMG_API ABall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABall();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VisualComponent;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:

	// Function that initializes the ball's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Returns ball Velocity vector.
	FVector GetVelocity() const;

	// Stops the ball setting its Projectile Movement Component Velocity vector to ZeroVector.
	void StopMovement();

private:
	// Returns 'value' to a value between [-1.0, 1.0].
	float Reduce(float value) const;

};
