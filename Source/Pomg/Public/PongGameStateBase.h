// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameStateBase.generated.h"

class UUserWidget;
class ABall;
class AAIPaddle;
class APaddle;

// Game states.
enum class EPongStates
{
	EWaitingToStart,
	EPlaying,
	EEnded
};

// Who has made the last score.
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

	// Start the game.
	void StartGame();

	// Add one point to the AI because it has made a score.
	void AddAIPoint();

	// Add one point to the Player because she/he has made a score.
	void AddPlayerPoint();

	// Launch the ball to the Player's paddle.
	void LaunchTheBallToPlayer();

	// Launch the ball to the AI's paddle.
	void LaunchTheBallToAI();

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

	// Current AI score.
	int AIScore;
	// Current Player score.
	int PlayerScore;

	// Who has made the last score.
	ELastScored LastScore;

	// Maximum score to win the game.
	int MaxScore;

	// Get level actors references (both paddle and the ball) and set the ball to its initial location.
	void InitLevelActors();

	// Check if someone has reached the maximum score.
	bool IsGameEnded();

	// Do all the stuff related to when the game has ended: show end menu, pause the game, ...
	void GameEnded();

	// Update the HUD to show a new score.
	void UpdateHud();

	// Changes the state to Playing and Launch the ball.
	void StartPlaying();

	// Launch the ball to YDirection (to the Player or to the AI paddle).
	void LaunchTheBall(float YDirection);

	// Stop the ball and moves it to its initial location.
	void ResetTheBall();
};
