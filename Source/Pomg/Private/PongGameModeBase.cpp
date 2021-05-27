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
		APongGameStateBase* PongGameState = World->GetGameState<APongGameStateBase>();

		PongGameState->StartGame();
	}
}
