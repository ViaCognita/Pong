// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PongHUD.generated.h"

/**
 * 
 */
UCLASS()
class POMG_API APongHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD Properties")
	UFont* ScoreFont;
	UFont* TextFont;

	APongHUD();

	virtual void DrawHUD() override;

	void SetAIScored();
	void SetPlayerScored();

private:
	int AIScore;
	int PlayerScore;
	
};
