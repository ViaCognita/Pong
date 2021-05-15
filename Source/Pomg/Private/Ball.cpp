// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Paddle.h"
#include "AIPaddle.h"
#include "Bound.h"
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
	PrimaryActorTick.bCanEverTick = true;

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

	RootComponent = CollisionComponent;
	VisualComponent->SetupAttachment(CollisionComponent);
	
	VisualComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // ECC_GameTraceChannel1 is my Projectile Object Type.
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block); // ECC_GameTraceChannel2 is my Bound Object Type.
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap); // ECC_GameTraceChannel3 is my Goal Object Type.
	CollisionComponent->SetGenerateOverlapEvents(true);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	// Use this component to drive this ball's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 0.0f;
	ProjectileMovementComponent->MaxSpeed = 190.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

		if (APaddle* Paddle = Cast<APaddle>(OtherActor))
		{
			float PaddleVelocity = ABall::Reduce(Paddle->GetZVelocity());

			//if (PaddleVelocity != 0.0f)
			//	PaddleVelocity = 1.0f;

			FVector Direction = FVector(0.0f, 1.0f, PaddleVelocity);

			ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->MaxSpeed;

			//Paddle->PlayHitSound();
		}
		else if(AAIPaddle* AIPaddle = Cast<AAIPaddle>(OtherActor))
		{
			float PaddleVelocity = ABall::Reduce(AIPaddle->GetZVelocity());

			//if (PaddleVelocity != 0.0f)
			//	PaddleVelocity = 1.0f;

			FVector Direction = FVector(0.0f, -1.0f, PaddleVelocity);

			ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->MaxSpeed;
			/*AAIPaddle* paddle = (AAIPaddle*)OtherActor;
			float paddleVelocity = paddle->GetZVelocity();

			if (paddleVelocity != 0.0f)
				ProjectileMovementComponent->Velocity = FVector(0.0f, 300.0f, 300.0f * paddleVelocity);

			paddle->PlayHitSound();*/
		}
		else
		{
			ABound* bounds = Cast<ABound>(OtherActor);
			// Reflecting vector: http://www.3dkingdoms.com/weekly/weekly.php?a=2
			FVector ReflectedVelocity = (-2 * FVector::DotProduct(ProjectileMovementComponent->Velocity, NormalImpulse) * NormalImpulse + ProjectileMovementComponent->MaxSpeed);
			// We only move on Y and Z axis.
			ReflectedVelocity.X = 0.0f;

			ProjectileMovementComponent->Velocity = ReflectedVelocity;

			//bounds->PlayHitSound();
		}
	}
}

void ABall::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlaps: %s"), *OtherActor->GetName()));
}

float ABall::Reduce(float value) const
{
	if (value > 1.0f)
		return 1.0f;
	else if (value < -1.0f)
		return -1.0f;
	else
		return value;
}

