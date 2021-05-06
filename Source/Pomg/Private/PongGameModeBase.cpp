// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameModeBase.h"
#include "AIPaddle.h"
#include "Paddle.h"
#include "Ball.h"
#include "PongGameStateBase.h"
#include "PongHUD.h"

APongGameModeBase::APongGameModeBase()
{
	DefaultPawnClass = APaddle::StaticClass();

	GameStateClass = APongGameStateBase::StaticClass();

	// HUD class this game uses.
	// We don't need to do anything else to use our HUD in the Game.
	HUDClass = APongHUD::StaticClass();
}


void APongGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		// Spawn the ball into the world.
		CurrentBall = World->SpawnActor<ABall>(ABall::StaticClass(), FVector(-710.0f, 0.0f, 250.0f), FRotator::ZeroRotator);
		FVector LaunchDirection(0.0f, -1.0f, 0.0f);
		CurrentBall->FireInDirection(LaunchDirection);

		APaddle* playerPaddle = (APaddle*)World->GetFirstPlayerController()->GetPawn();
		playerPaddle->SetActorLocation(FVector(0.0f, -800.0f, 250.0f));
		playerPaddle->SetActorRotation(FRotator::ZeroRotator);

		AAIPaddle* aiPaddle = World->SpawnActor<AAIPaddle>(AAIPaddle::StaticClass(), FVector(0.0f, 800.0f, 250.0f), FRotator::ZeroRotator);

		//aiPaddle->SetBall(CurrentBall);
	}
}

ABall* APongGameModeBase::GetBall()
{
	return CurrentBall;
}
