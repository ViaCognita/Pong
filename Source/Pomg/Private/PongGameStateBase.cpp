// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameStateBase.h"
#include "Ball.h"
#include "PongGameModeBase.h"

APongGameStateBase::APongGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EPongStates::EWaitingToStart;

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
					ABall* ball = (Cast<APongGameModeBase>(world->GetAuthGameMode()))->GetBall();
					//ball->StartMovement();
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
