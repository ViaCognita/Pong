// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Paddle.h"
#include "AIPaddle.h"
#include "PongGameStateBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return std::is_base_of<Base, T>::value;
}

// Sets default values
ABall::ABall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualComp"));
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Game/Geometry/Meshes/Ball_mesh.Ball_mesh"));

	if (VisualAsset.Succeeded())
	{
		VisualComponent->SetStaticMesh(VisualAsset.Object);
		VisualComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		VisualComponent->SetWorldScale3D(FVector(1.0f));

		FBoxSphereBounds Bounds = VisualAsset.Object->ExtendedBounds;

		CollisionComponent->SetRelativeLocation(Bounds.Origin);
		CollisionComponent->SetBoxExtent(Bounds.BoxExtent);
	}

	// Set Collision component as Root Component.
	RootComponent = CollisionComponent;
	// Attach Visual Component to Collision Component.
	VisualComponent->SetupAttachment(CollisionComponent);

	// Disable collisions in Visual Component.
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Enable Query Only Collisions in the Collision component.
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Set collision object type to Projectile.
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // ECC_GameTraceChannel1 is Projectile Collision Object Type.
	// Set response to all channels to ignore.
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Set block response to Pawn's object channel.
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	// Set block response to Frontier's object channel.
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block); // ECC_GameTraceChannel2 is Frontier Collision Object Type.
	// Set overlap response to Objective's object channel.
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap); // ECC_GameTraceChannel3 is Objective Collision Object Type.
	// Set to generate overlap events.
	CollisionComponent->SetGenerateOverlapEvents(true);
	// Set the method to execute when a hit event it's triggered.
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	// Use this component to drive ball's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 190.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void ABall::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Check if property is valid
	if (PropertyChangedEvent.Property != nullptr)
	{
		// Get the name of the changed property
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());
		// If the changed property is ShowStaticMesh then we
		// will set the visibility of the actor
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ABall, VisualComponent))
		{
			UStaticMesh* SM = VisualComponent->GetStaticMesh();
			if (SM != nullptr)
			{
				FBoxSphereBounds Bounds = SM->ExtendedBounds;

				CollisionComponent->SetRelativeLocation(Bounds.Origin);
				CollisionComponent->SetBoxExtent(Bounds.BoxExtent);
			}
		}
	}
	// Then call the parent version of this function
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void ABall::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->MaxSpeed;
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ball Hits: %s"), *OtherActor->GetName()));

		// Compute Reflected Velocity depending on the actor with whom we have collided.
		FVector ReflectedVelocity;

		// Paddle Hit.
		if (APaddle* Paddle = Cast<APaddle>(OtherActor))
		{
			float PaddleVelocity = Paddle->GetZVelocity();

			// The ball only moves on axis Y, and Z.
			// Here, Y = 1.0 because this paddle is on the left side and the ball will bounce to the right side of the screen.
			// PaddleVelocity is Paddle's Z velocity.
			// If Paddle is moving down (PaddleVelocity < 0.0), the ball will bounces down.
			// If Paddle is moving up (PaddleVelocity > 0.0), the ball will bounces up.
			// This is why I use PaddleVelocity as the new Z velocity for the ball.
			FVector Direction = FVector(0.0f, 1.0f, PaddleVelocity);

			ReflectedVelocity = Direction * ProjectileMovementComponent->MaxSpeed;
		}
		// AI Paddle Hit.
		else if (AAIPaddle* AIPaddle = Cast<AAIPaddle>(OtherActor))
		{
			float PaddleVelocity = AIPaddle->GetZVelocity();

			// Here, Y = 1.0 because this paddle is on the right side and the ball will bounce to the left side of the screen.
			FVector Direction = FVector(0.0f, -1.0f, PaddleVelocity);

			ReflectedVelocity = Direction * ProjectileMovementComponent->MaxSpeed;
		}
		// Frontier Hit.
		else
		{
			FVector BallVelocity = ProjectileMovementComponent->Velocity;
			// The ball will continue moving on the same direction on Y axis.
			// But it will change its direction on Z axis because the edges are at the top and at the bottom of the screen.
			// This is why we change the sign of BallVelocity.Z.
			// The Z axis is the vertical axis of the screen(up and down).
			ReflectedVelocity = FVector(BallVelocity.X, BallVelocity.Y, -BallVelocity.Z);
		}

		// If the ball won't move up or down, we will choose randomly if it will move up or down.
		// This happens when the ball hits a paddle that is not moving up or down.
		if (ReflectedVelocity.Z == 0.0f)
		{
			float random = FMath::RandRange(-1.0f, 1.0f);

			// if it is negative, it will move down.
			if (random < 0.0f)
				ReflectedVelocity.Z = -1.0f * ProjectileMovementComponent->MaxSpeed;
			// else, if it is positive, it will move up.
			else
				ReflectedVelocity.Z = 1.0f * ProjectileMovementComponent->MaxSpeed;
		}

		// Set the new velocity to the ball's movement component.
		ProjectileMovementComponent->Velocity = ReflectedVelocity;
		
		UE_LOG(LogTemp, Warning, TEXT("Current values are: ReflectedVelocity %s"), *ReflectedVelocity.ToString());
	}
}

void ABall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlaps: %s"), *OtherActor->GetName()));

	// If the ball has overlaped the AI Goal, then AI makes a point.
	if (OtherActor->GetName().Equals("Goal_AI"))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Goal_AI"));

		UWorld* const world = GetWorld();
		if (world)
		{
			APongGameStateBase* GameState = Cast<APongGameStateBase>(world->GetGameState());

			// Add one point to AI score.
			GameState->AddAIPoint();
		}
	}
	// If the ball has overlaped the Player Goal, then Player makes a point.
	else if (OtherActor->GetName().Equals("Goal_Player"))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Goal_Player"));

		UWorld* const world = GetWorld();
		if (world)
		{
			APongGameStateBase* GameState = Cast<APongGameStateBase>(world->GetGameState());

			// Add one point to player score.
			GameState->AddPlayerPoint();
		}
	}
}

FVector ABall::GetVelocity() const
{
	return ProjectileMovementComponent->Velocity;
}

void ABall::StopMovement()
{
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
}

float ABall::Reduce(float value) const
{
	if (value >= 1.0f)
		return 1.0f;
	else if (value <= -1.0f)
		return -1.0f;
	else
		return 0.0f;
}

