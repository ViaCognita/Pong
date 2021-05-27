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
	// Default Pawn for this game.
	DefaultPawnClass = APaddle::StaticClass();

	// Game State class.
	GameStateClass = APongGameStateBase::StaticClass();

	// HUD class this game uses.
	HUDClass = APongHUD::StaticClass();
}


void APongGameModeBase::StartPlay()
{
	Super::StartPlay();

	UWorld* const World = GetWorld();
	if (World)
	{
		APongGameStateBase* PongGameState = World->GetGameState<APongGameStateBase>();

		// Init the game on Game State.
		PongGameState->StartGame();
	}
}
