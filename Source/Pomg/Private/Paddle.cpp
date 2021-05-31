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
	
	VisualComponent->BodyInstance.SetCollisionProfileName("NoCollision");
	CollisionComponent->BodyInstance.SetCollisionProfileName("Pawn");
	CollisionComponent->OnComponentHit.AddDynamic(this, &APaddle::OnHit);

	// Initialize paddle velocity.
	ZDirection = 0.0f;

	// Create an instance of our movement component, and tell it to update our root component.
	OurMovementComponent = CreateDefaultSubobject<UPaddlePawnMovementComponent>(TEXT("PaddleCustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	// Create the sound.
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
	// Check if property is valid
	if (PropertyChangedEvent.Property != nullptr)
	{
		// Get the name of the changed property
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		// If the changed property is ShowStaticMesh then we
		// will set the visibility of the actor
		if (PropertyName == GET_MEMBER_NAME_CHECKED(APaddle, VisualComponent))
		{
			UStaticMesh* SM = VisualComponent->GetStaticMesh();
			if (SM != nullptr)
			{
				FBoxSphereBounds Bounds = SM->ExtendedBounds;

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

	// Respond every frame to the values of our movement.
	InputComponent->BindAxis(TEXT("MovePaddle"), this, &APaddle::Move_ZAxis);
}

void APaddle::Move_ZAxis(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		ZDirection = AxisValue;

		float Scale = 100.0f; // TODO mover a propiedad para poder modificarla en el editor.

		FVector DirectionVector = FVector(0.0f, 0.0f, AxisValue);

		OurMovementComponent->AddInputVector(DirectionVector * Scale);
	}
}

float APaddle::GetZVelocity() const
{
	return ZDirection;
}
