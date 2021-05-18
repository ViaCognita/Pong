// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameModeBase.generated.h"

class ABall;

/**
 * 
 */
UCLASS()
class POMG_API APongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	APongGameModeBase();

	virtual void StartPlay() override;

	void LaunchTheBallToPlayer();
	void LaunchTheBallToAI();

	void StopTheBall();

private:
	
	ABall* CurrentBall;

	void LaunchTheBall(float YDirection);
	
};
