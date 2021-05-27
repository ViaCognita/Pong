// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AIPaddle.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class UPaddlePawnMovementComponent;
class ABall;
class USoundBase;

UCLASS()
class POMG_API AAIPaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAIPaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used to detect collision.s
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	// Used to show a static mesh.
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VisualComponent;

	// Input variables
	FVector CurrentVelocity;

	// Use my own Pawn Movement Component implementation.
	UPaddlePawnMovementComponent* OurMovementComponent;

	// Get Pawn Movement Component.
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	// Event triggered when a property has been changed in the Unreal editor.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	// The ball in the game.
	ABall* GameBall;

	// Compute towards which Z coordinate the ball is moving.
	float ComputeBallZCoordinate() const;

	// Move the paddle to 'direction'.
	void MovePaddle(float direction);

	// Sound played when the ball hits this Paddle.
	USoundBase* HitSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set the reference for the ball in the game.
	void SetGameBallReference(ABall* Ball);

	/**
	* Velocity is a vector quantity that refers to "the rate at which an object changes its position."
	* Description taken from: https://www.physicsclassroom.com/class/1DKin/Lesson-1/Speed-and-Velocity
	*/
	float GetZVelocity() const;

};
