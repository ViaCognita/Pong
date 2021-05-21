// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameModeBase.h"
#include "AIPaddle.h"
#include "Paddle.h"
#include "Ball.h"
#include "PongGameStateBase.h"
#include "PongHUD.h"
#include "Kismet/GameplayStatics.h"


APongGameModeBase::APongGameModeBase()
{
	DefaultPawnClass = APaddle::StaticClass();

	GameStateClass = APongGameStateBase::StaticClass();

	// HUD class this game uses.
	// We don't need to do anything else to use our HUD in the Game.
	HUDClass = APongHUD::StaticClass();

	BallStartLocation = FVector(0.0f, 0.0f, 250.0f);
}


void APongGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* const World = GetWorld();
	if (World)
	{		
		// Spawn the ball into the world.
		CurrentBall = World->SpawnActor<ABall>(ABall::StaticClass(), BallStartLocation, FRotator::ZeroRotator);

		LaunchTheBallToPlayer();

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPaddle::StaticClass(), FoundActors);

		for (AActor* TActor : FoundActors)
		{
			AAIPaddle* AIPaddle = Cast<AAIPaddle>(TActor);

			if (AIPaddle != nullptr)
				AIPaddle->SetGameBall(CurrentBall);
		}
	}
}

void APongGameModeBase::LaunchTheBallToPlayer()
{
	LaunchTheBall(-1.0f);
}

void APongGameModeBase::LaunchTheBallToAI()
{
	LaunchTheBall(1.0f);
}

void APongGameModeBase::ResetTheBall()
{
	CurrentBall->StopMovement();

	CurrentBall->SetActorLocation(BallStartLocation);
}

void APongGameModeBase::LaunchTheBall(float YDirection)
{
	if (CurrentBall != nullptr)
	{
		FVector LaunchDirection(0.0f, YDirection, 0.0f);
		CurrentBall->FireInDirection(LaunchDirection);
	}
}

