// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PongHUD.generated.h"

class UPlayerHUDWidget;
class UUserWidget;

/**
 * 
 */
UCLASS()
class POMG_API APongHUD : public AHUD
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD Properties")
	UFont* ScoreFont;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD Properties")
	UFont* TextFont;

	APongHUD();

	void SetAIScored(int value);
	void SetPlayerScored(int value);

private:

	// HUD Widgent parent class.
	UPlayerHUDWidget* PlayerHudWidget;

	// HUD Widget class.
	TSubclassOf<UUserWidget> HudClass;

	// HUD Widget instance.
	UUserWidget* Widget;
};
