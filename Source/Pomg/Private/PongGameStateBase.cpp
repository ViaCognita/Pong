// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameStateBase.h"
#include "Ball.h"
#include "PongGameModeBase.h"
#include "PongHUD.h"

APongGameStateBase::APongGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EPongStates::EWaitingToStart;

	AIScore = 0;
	PlayerScore = 0;

	LastScore = ELastScored::ENone;
}

void APongGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	// Get Player's controller.
	Controller = GetWorld()->GetFirstPlayerController();
}

void APongGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentState)
	{
		case EPongStates::EWaitingToStart:
			if (Controller->WasInputKeyJustReleased(EKeys::SpaceBar))
			{
				CurrentState = EPongStates::EPushBall;
				
				UWorld* const world = GetWorld();
				if (world)
				{
					APongGameModeBase* GameMode = Cast<APongGameModeBase>(world->GetAuthGameMode());

					if (LastScore == ELastScored::EArtificialIntelligence)
						GameMode->LaunchTheBallToAI();
					else
						GameMode->LaunchTheBallToPlayer();
				}
			}
			break;
		case EPongStates::EPushBall:
			//SpawnBall();
			break;
		case EPongStates::EPlaying:
			break;
		case EPongStates::EReset:
			break;
	}

}

EPongStates APongGameStateBase::GetCurrentState()
{
    return CurrentState;
}

void APongGameStateBase::AddAIPoint()
{
	APongGameModeBase* GameMode = Cast<APongGameModeBase>(GetWorld()->GetAuthGameMode());

	// Increase AI score.
	AIScore++;

	// Update who has made the last score.
	LastScore = ELastScored::EArtificialIntelligence;

	// Stop the ball.
	GameMode->StopTheBall();

	// Update the HUD.
	APongHUD* Hud = Cast<APongHUD>(GameMode->HUDClass);

	Hud->SetAIScored(AIScore);

	// Change game state.
	CurrentState = EPongStates::EWaitingToStart;
}

void APongGameStateBase::AddPlayerPoint()
{
	APongGameModeBase* GameMode = Cast<APongGameModeBase>(GetWorld()->GetAuthGameMode());

	// Increase AI score.
	PlayerScore++;

	// Update who has made the last score.
	LastScore = ELastScored::EPlayer;

	// Stop the ball.
	GameMode->StopTheBall();

	// Update the HUD.

	// Change game state.
	CurrentState = EPongStates::EWaitingToStart;
}
