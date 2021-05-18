// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PongGameStateBase.generated.h"


enum class EPongStates
{
	EReset,
	EWaitingToStart,
	EPlaying,
	EPushBall
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

	//Called when play begins
	virtual void BeginPlay() override;

	// Tick called every frame
	virtual void Tick(float DeltaTime) override;

	// Get current state
	EPongStates GetCurrentState();

	void AddAIPoint();

	void AddPlayerPoint();

private:

	EPongStates CurrentState;

	// Reference to the controller used for the paddle
	class APlayerController* Controller;

	// Values to store the current score
	int AIScore;
	int PlayerScore;

	// Who has made the last score.
	ELastScored LastScore;
};
