// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameStateBase.generated.h"

class UUserWidget;


enum class EPongStates
{
	EWaitingToStart,
	EPlaying,
	EPaused,
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

	//Called when play begins
	virtual void BeginPlay() override;

	// Tick called every frame
	virtual void Tick(float DeltaTime) override;

	// Get current state
	EPongStates GetCurrentState();

	void AddAIPoint();

	void AddPlayerPoint();

	void ResetGame();

private:
	TSubclassOf<UUserWidget> MenuClass;
	
	EPongStates CurrentState;

	// Reference to the controller used for the paddle
	class APlayerController* Controller;

	// Values to store the current score
	int AIScore;
	int PlayerScore;

	// Who has made the last score.
	ELastScored LastScore;

	int MaxScore;

	bool IsGameEnded();

	void GameEnded();

	void UpdateHud();

	void StartPlaying();
};
