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
	OurMovementComponent = CreateDefaultSubobject<UPaddlePawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void AAIPaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

