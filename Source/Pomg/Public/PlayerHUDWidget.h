// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

/**
 *
 */
UCLASS()
class POMG_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Player_Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AI_Score;

	// Set 'NewScore' as the score for the Player.
	void SetPlayerScore(int NewScore);

	// Set 'NewScore' as the score for the AI.
	void SetAIScore(int NewScore);
};
