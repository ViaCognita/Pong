// Fill out your copyright notice in the Description page of Project Settings.


#include "Paddle.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PaddlePawnMovementComponent.h"

// Sets default values
APaddle::APaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComp"));
	RootComponent = VisualComponent;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollComp"));
	// No physics, queries only.
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// It will generate the 'Hit' event.
	CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	CollisionComponent->SetupAttachment(VisualComponent);

	// Disable collision.
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Game/Geometry/Meshes/Paddle_mesh.Paddle_mesh"));
	if (VisualAsset.Succeeded())
	{
		VisualComponent->SetStaticMesh(VisualAsset.Object);
		VisualComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualComponent->SetWorldScale3D(FVector(1.0f));
	}

	// Initialize paddle velocity.
	CurrentVelocity.Z = 0.0f;

	// Create an instance of our movement component, and tell it to update our root component.
	OurMovementComponent = CreateDefaultSubobject<UPaddlePawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void APaddle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle movement based on our "MoveZ" axis.
	if (!CurrentVelocity.IsZero())
	{
		const FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}

}

UPawnMovementComponent* APaddle::GetMovementComponent() const
{
	return OurMovementComponent;
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
	CurrentVelocity.Z = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

