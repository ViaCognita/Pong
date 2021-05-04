// Fill out your copyright notice in the Description page of Project Settings.


#include "Bound.h"
#include "Components/BoxComponent.h"

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

	RootComponent = CollisionComponent;

}

// Called when the game starts or when spawned
void ABound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

