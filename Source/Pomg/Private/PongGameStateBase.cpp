// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameStateBase.h"
#include "Ball.h"
#include "AIPaddle.h"
#include "Paddle.h"
#include "PongGameModeBase.h"
#include "PongHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "EndMenuWidget.h"

APongGameStateBase::APongGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentState = EPongStates::EWaitingToStart;

	AIScore = 0;
	PlayerScore = 0;

	MaxScore = 1;

	LastScore = ELastScored::ENone;
	
	ConstructorHelpers::FClassFinder<UUserWidget> WBPMenuFinder(TEXT("/Game/Blueprint/Widgets/UI/UMG_EndMenu"));
	if (WBPMenuFinder.Succeeded())
	{
		MenuClass = WBPMenuFinder.Class;
	}
}

void APongGameStateBase::StartGame()
{
	// Reset the scores and update the HUD.
	PlayerScore = 0;
	AIScore = 0;

	UpdateHud();

	// Change game state to Waiting to start.
	CurrentState = EPongStates::EWaitingToStart;

	// Move paddles to initial location.
	FVector PlayerLocation(0.0f, -500.0f, 170.0f);
	FVector AILocation(0.0f, 500.0f, 170.0f);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (AActor* TActor : FoundActors)
	{
		if (AAIPaddle* AIPaddle = Cast<AAIPaddle>(TActor))
			AIPaddle->SetActorLocation(AILocation);
		else if (APaddle* PlayerPaddle = Cast<APaddle>(TActor))
			PlayerPaddle->SetActorLocation(PlayerLocation);
	}

	// Enable the keyboard to let the player play.
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Disable mouse cursor.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

	// Unset the game to pause.
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(false);

	// Launch the ball
	StartPlaying();
}

void APongGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	// Get Player's controller.
	Controller = GetWorld()->GetFirstPlayerController();
}

void APongGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Change game state.
	if (CurrentState != EPongStates::EPaused)
	{
		if (IsGameEnded())
			CurrentState = EPongStates::EEnded;
		else
			CurrentState = EPongStates::EWaitingToStart;

		switch (CurrentState)
		{
		case EPongStates::EWaitingToStart:
			if (Controller->WasInputKeyJustReleased(EKeys::SpaceBar))
			{
				StartPlaying();
			}
			break;
		case EPongStates::EPlaying:
			break;
		case EPongStates::EEnded:
			CurrentState = EPongStates::EPaused;
			GameEnded();
			break;
		}
	}

}

EPongStates APongGameStateBase::GetCurrentState()
{
    return CurrentState;
}

void APongGameStateBase::AddAIPoint()
{
	// Increase AI Score.
	AIScore++;

	UpdateHud();
}

void APongGameStateBase::AddPlayerPoint()
{
	// Increase Player Score.
	PlayerScore++;

	UpdateHud();
}

void APongGameStateBase::ResetGame()
{
}

bool APongGameStateBase::IsGameEnded()
{
	if ((PlayerScore == MaxScore) || (AIScore == MaxScore))
		return true;
	else
		return false;
}

void APongGameStateBase::GameEnded()
{
	if (MenuClass)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(true);

		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);

		Widget->AddToViewport();

		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		UEndMenuWidget* MenuWidget = Cast<UEndMenuWidget>(Widget);

		if (AIScore == MaxScore)
			MenuWidget->SetEndMessage(FText::FromString("You lose!"));
		else
			MenuWidget->SetEndMessage(FText::FromString("You Win!"));

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
	}

}

void APongGameStateBase::UpdateHud()
{
	APongGameModeBase* GameMode = Cast<APongGameModeBase>(GetWorld()->GetAuthGameMode());

	// Update who has made the last score.
	LastScore = ELastScored::EPlayer;

	// Stop the ball and move it to its start location.
	GameMode->ResetTheBall();

	// Update the HUD.
	APongHUD* Hud = Cast<APongHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());

	Hud->SetPlayerScored(PlayerScore);
	Hud->SetAIScored(AIScore);
}

void APongGameStateBase::StartPlaying()
{
	CurrentState = EPongStates::EPlaying;

	UWorld* const world = GetWorld();
	if (world)
	{
		APongGameModeBase* GameMode = Cast<APongGameModeBase>(world->GetAuthGameMode());

		if (LastScore == ELastScored::EArtificialIntelligence)
			GameMode->LaunchTheBallToAI();
		else
			GameMode->LaunchTheBallToPlayer();
	}

}
