// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGoal::AGoal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollComp"));
	CollisionComponent->SetBoxExtent(FVector(30.0f, 30.0f, 30.0f));

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel3);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	CollisionComponent->SetGenerateOverlapEvents(true);

	RootComponent = CollisionComponent;

	// Create the sound.
	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Game/Effects/pong-point.pong-point"));
	if (HitSoundAsset.Succeeded())
		HitSound = HitSoundAsset.Object;

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Overlaps: %s"), *OtherActor->GetName()));

	// When the ball hits this, play a sound.
	if (ABall* Ball = Cast<ABall>(OtherActor))
	{
		if (HitSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		}
	}
}

