// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Paddle.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class UPaddlePawnMovementComponent;
class USoundBase;

UCLASS()
class POMG_API APaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaddle();

protected:

	// Used to detect collisions.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	// Used to show a static mesh and make it visible.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VisualComponent;

	// Keep current Z velocity.
	float ZVelocity;

	// Use my own Pawn Movement Component implementation.
	UPaddlePawnMovementComponent* OurMovementComponent;

	// Get Pawn Movement Component.
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Event triggered when a property has been changed in the Unreal editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Player's input method.
	void Move_ZAxis(float AxisValue);

	/**
	* Velocity is a vector quantity that refers to "the rate at which an object changes its position."
	* From: https://www.physicsclassroom.com/class/1DKin/Lesson-1/Speed-and-Velocity
	*/
	float GetZVelocity() const;

private:
	// Sound played when the ball hits this paddle.
	USoundBase* HitSound;

};
