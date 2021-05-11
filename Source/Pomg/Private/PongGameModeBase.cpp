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
}


void APongGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		
		// Spawn the ball into the world.
		CurrentBall = World->SpawnActor<ABall>(ABall::StaticClass(), FVector(0.0f, 0.0f, 250.0f), FRotator::ZeroRotator);
		FVector LaunchDirection(0.0f, -1.0f, 0.0f);
		CurrentBall->FireInDirection(LaunchDirection);
		
		/*
		APaddle* playerPaddle = (APaddle*)World->GetFirstPlayerController()->GetPawn();
		playerPaddle->SetActorLocation(FVector(0.0f, -500.0f, 170.0f));
		playerPaddle->SetActorRotation(FRotator::ZeroRotator);

		AAIPaddle* aiPaddle = World->SpawnActor<AAIPaddle>(AAIPaddle::StaticClass(), FVector(0.0f, 500.0f, 170.0f), FRotator::ZeroRotator);
		*/

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPaddle::StaticClass(), FoundActors);

		for (AActor* TActor : FoundActors)
		{
			AAIPaddle* AIPaddle = Cast<AAIPaddle>(TActor);

			if (AIPaddle != nullptr)
				// Do what ever you need with your MyActor
				AIPaddle->SetGameBall(CurrentBall);
		}

		//aiPaddle->SetBall(CurrentBall);
	}
}

ABall* APongGameModeBase::GetBall()
{
	return CurrentBall;
}
