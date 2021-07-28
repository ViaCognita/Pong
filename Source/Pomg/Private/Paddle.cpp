// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PaddlePawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APaddle::APaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PaddleCollisionComponent"));
	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleVisualComponent"));

	RootComponent = CollisionComponent;

	// Set up CollisionComponent as parent of VisualComponent;
	VisualComponent->SetupAttachment(CollisionComponent);
	
	// Disable collisions on Visual Component using "NoCollision" profile.
	VisualComponent->BodyInstance.SetCollisionProfileName("NoCollision");
	// Enable collisions on Collision Component using "Pawn" profile.
	CollisionComponent->BodyInstance.SetCollisionProfileName("Pawn");
	// Add method to manage collisions.
	CollisionComponent->OnComponentHit.AddDynamic(this, &APaddle::OnHit);

	// Initialize paddle velocity.
	ZVelocity = 0.0f;

	// Create an instance of our movement component, and tell it to update our root component.
	OurMovementComponent = CreateDefaultSubobject<UPaddlePawnMovementComponent>(TEXT("PaddleCustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	// Create the sound to play it when the ball hit this paddle.
	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Game/Effects/pong-paddle.pong-paddle"));
	if (HitSoundAsset.Succeeded())
		HitSound = HitSoundAsset.Object;

}

UPawnMovementComponent* APaddle::GetMovementComponent() const
{
	return OurMovementComponent;
}

void APaddle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Check if the property changed is valid.
	if (PropertyChangedEvent.Property != nullptr)
	{
		// Get the name of the changed property
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		
		// If the changed property is Static Mesh then we can set
		// CollisionComponent extend to the same size of the new Static Mesh
		// set to the VisualComponent.
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APaddle, VisualComponent))
		{
			// Get the new Static Mesh set to the Visual Component.
			UStaticMesh* SM = VisualComponent->GetStaticMesh();
			// If it is valid.
			if (SM != nullptr)
			{
				// Get the Static Mesh Bounds.
				FBoxSphereBounds Bounds = SM->ExtendedBounds;

				// Set this Bounds to the Collision Component to 
				CollisionComponent->SetBoxExtent(Bounds.BoxExtent);
			}
		}
	}

	// Then call the parent version of this function
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void APaddle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Paddle has been hit by: %s"), *OtherActor->GetName()));

		if (ABall* Ball = Cast<ABall>(OtherActor))
		{
			if (HitSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
			}
		}
	}
}

// Called to bind functionality to input
void APaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind "MovePaddle" Axis Mapping with APaddle::Move_ZAxis method.
	InputComponent->BindAxis(TEXT("MovePaddle"), this, &APaddle::Move_ZAxis);
}

void APaddle::Move_ZAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		// Keep the lastest Z velocity to check it when the ball hit this paddle.
		ZVelocity = AxisValue;

		float Scale = 100.0f; // TODO mover a propiedad para poder modificarla en el editor.

		FVector DirectionVector = FVector(0.0f, 0.0f, AxisValue);

		OurMovementComponent->AddInputVector(DirectionVector * Scale);
	}
}

float APaddle::GetZVelocity() const
{
	return ZVelocity;
}
