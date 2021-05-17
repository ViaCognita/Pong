// Fill out your copyright notice in the Description page of Project Settings.


#include "AIPaddle.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PaddlePawnMovementComponent.h"

// Avoid add unnecesary imports.
class UBoxComponent;
class UPaddlePawnMovementComponent;

// Sets default values
AAIPaddle::AAIPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollComp"));
	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComp"));

	RootComponent = CollisionComponent;

	// Set up CollisionComponent as parent of VisualComponent;
	VisualComponent->SetupAttachment(CollisionComponent);

	VisualComponent->BodyInstance.SetCollisionProfileName("NoCollision");
	CollisionComponent->BodyInstance.SetCollisionProfileName("Pawn");

	// Initialize paddle velocity.
	CurrentVelocity.Z = 0.0f;

	// Create an instance of our movement component, and tell it to update our root component.
	OurMovementComponent = CreateDefaultSubobject<UPaddlePawnMovementComponent>(TEXT("AIPaddleCustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void AAIPaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

float AAIPaddle::ComputeBallZCoordinate() const
{
	float z = 0.0f;
	// 1. Get paddle location.
	FVector PaddleLocation = GetActorLocation();
	
	// 2. Get ball location.
	FVector BallLocation = GameBall->GetActorLocation();
	
	// 3. Get ball velocity.
	FVector BallVelocty = GameBall->GetVelocity();

	// 4. Compute ball new Z location.
	float t = (PaddleLocation.Y - BallLocation.Y) / BallVelocty.Y;
	
	z = BallVelocty.Z * t + BallLocation.Z;
	
	return z;
}

void AAIPaddle::MovePaddle(float direction)
{
	float Scale = 100.0f;

	FVector DirectionVector = FVector(0.0f, 0.0f, direction);

	OurMovementComponent->AddInputVector(DirectionVector * Scale);
}

// Called every frame
void AAIPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameBall->GetVelocity().Y > 0.0f)
	{
		float z = ComputeBallZCoordinate();

		if (z > GetActorLocation().Z)
			MovePaddle(1.0f);
		else if (z < GetActorLocation().Z)
			MovePaddle(-1.0f);
		else
			MovePaddle(0.0f);
	}

}

// Called to bind functionality to input
void AAIPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAIPaddle::SetGameBall(ABall* Ball)
{
	this->GameBall = Ball;
}

UPawnMovementComponent* AAIPaddle::GetMovementComponent() const
{
	return OurMovementComponent;
}

void AAIPaddle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Check if property is valid
	if (PropertyChangedEvent.Property != nullptr)
	{
		// Get the name of the changed property
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		// If the changed property is ShowStaticMesh then we
		// will set the visibility of the actor
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AAIPaddle, VisualComponent))
		{
			UStaticMesh* SM = VisualComponent->GetStaticMesh();
			if (SM != nullptr)
			{
				FBoxSphereBounds Bounds = SM->ExtendedBounds;

				//CollisionComponent->SetRelativeLocation(Bounds.Origin);
				CollisionComponent->SetBoxExtent(Bounds.BoxExtent);
			}
		}
	}

	// Then call the parent version of this function
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

float AAIPaddle::GetZVelocity() const
{
	return CurrentVelocity.Z;
}

