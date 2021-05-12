// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Paddle.generated.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class UPaddlePawnMovementComponent;

UCLASS()
class POMG_API APaddle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APaddle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VisualComponent;

	// Input variables
	FVector CurrentVelocity;

	// Use my own Pawn Movement Component implementation.
	UPaddlePawnMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void Move_ZAxis(float AxisValue);

	/**
	* Velocity is a vector quantity that refers to "the rate at which an object changes its position."
	* From: https://www.physicsclassroom.com/class/1DKin/Lesson-1/Speed-and-Velocity
	*/
	float GetZVelocity() const;

};
