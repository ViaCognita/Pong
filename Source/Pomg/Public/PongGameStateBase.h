// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameStateBase.generated.h"

class UUserWidget;
class ABall;
class AAIPaddle;
class APaddle;


enum class EPongStates
{
	EWaitingToStart,
	EPlaying,
	EEnded
};

enum class ELastScored
{
	EPlayer,
	EArtificialIntelligence,
	ENone
};

/**
 * 
 */
UCLASS()
class POMG_API APongGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:

	APongGameStateBase();

	void StartGame();

	// Tick called every frame
	virtual void Tick(float DeltaTime) override;

	// Get current state
	EPongStates GetCurrentState();

	void AddAIPoint();

	void AddPlayerPoint();

	void ResetGame();

	void LaunchTheBallToPlayer();
	void LaunchTheBallToAI();

	void ResetTheBall();

private:
	// The ball.
	ABall* TheBall;

	// Ball's start location in level.
	FVector BallStartLocation;

	// Player's paddle start location.
	FVector PlayerLocation;
	// AI's paddle start location.
	FVector AILocation;

	// Player's paddle reference.
	APaddle* PlayerPaddle;

	// AI's paddle reference.
	AAIPaddle* AIPaddle;
	
	// Class for end game menu.
	TSubclassOf<UUserWidget> EndMenuClass;
	
	// Current game state.
	EPongStates CurrentState;

	// To know if this is the first time we start the game.
	bool IsFirstStart;

	// Values to store the current score
	int AIScore;
	int PlayerScore;

	// Who has made the last score.
	ELastScored LastScore;

	int MaxScore;

	void InitLevelActors();

	bool IsGameEnded();

	void GameEnded();

	void UpdateHud();

	void StartPlaying();

	void LaunchTheBall(float YDirection);
};
