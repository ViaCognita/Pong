// Fill out your copyright notice in the Description page of Project Settings.


#include "Bound.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABound::ABound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollComp"));
	CollisionComponent->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));
	
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABound::OnHit);

	RootComponent = CollisionComponent;

	// Create the sound.
	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Game/Effects/pong-wall.pong-wall"));
	if (HitSoundAsset.Succeeded())
		HitSound = HitSoundAsset.Object;

}

// Called when the game starts or when spawned
void ABound::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABound::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

// Called every frame
void ABound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

