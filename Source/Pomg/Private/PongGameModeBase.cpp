// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameModeBase.h"
#include "Paddle.h"
#include "Ball.h"


APongGameModeBase::APongGameModeBase()
{
	DefaultPawnClass = APaddle::StaticClass();
}


void APongGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		// Spawn the ball into the world.
		CurrentBall = World->SpawnActor<ABall>(ABall::StaticClass(), FVector(0.0f, 0.0f, 250.0f), FRotator::ZeroRotator);
	}
}
