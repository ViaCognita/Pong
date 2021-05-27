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

	// Initializate the ball and its start location.
	TheBall = nullptr;
	BallStartLocation = FVector(0.0f, 0.0f, 250.0f);

	// Initialize Paddle's initial location.
	PlayerLocation = FVector(0.0f, -500.0f, 170.0f);
	AILocation = FVector(0.0f, 500.0f, 170.0f);

	// This is the first start.
	IsFirstStart = true;

	// Initial state: waiting to start.
	CurrentState = EPongStates::EWaitingToStart;

	// Initial scores.
	AIScore = 0;
	PlayerScore = 0;

	// Maximum score.
	MaxScore = 1; //TODO: Ideally we could set this on Unreal editor.

	// How has scored last.
	LastScore = ELastScored::ENone;

	// End menu class.
	ConstructorHelpers::FClassFinder<UUserWidget> WBPEndMenuFinder(TEXT("/Game/Blueprint/Widgets/UI/UMG_EndMenu"));
	if (WBPEndMenuFinder.Succeeded())
	{
		EndMenuClass = WBPEndMenuFinder.Class;
	}
}

void APongGameStateBase::StartGame()
{
	if (IsFirstStart)
	{
		InitLevelActors();
		IsFirstStart = false;
	}
	else
	{
		// Reset the scores and update the HUD.
		PlayerScore = 0;
		AIScore = 0;

		UpdateHud();

		// Change game state to Waiting to start.
		CurrentState = EPongStates::EWaitingToStart;

		if (AIPaddle)
			AIPaddle->SetActorLocation(AILocation);
		if (PlayerPaddle)
			PlayerPaddle->SetActorLocation(PlayerLocation);

		// Enable the keyboard to let the player play.
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		// Disable mouse cursor.
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

		// Unset the game to pause.
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(false);
	}

	// Launch the ball
	StartPlaying();
}

void APongGameStateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Change game state.
	if (IsGameEnded())
		CurrentState = EPongStates::EEnded;
	else
		CurrentState = EPongStates::EWaitingToStart;

	switch (CurrentState)
	{
		case EPongStates::EWaitingToStart:
			break;
		case EPongStates::EPlaying:
			break;
		case EPongStates::EEnded:
			GameEnded();
			break;
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
	if (EndMenuClass)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(true);

		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), EndMenuClass);

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
	// Update who has made the last score.
	LastScore = ELastScored::EPlayer;

	// Stop the ball and move it to its start location.
	ResetTheBall();

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
		if (LastScore == ELastScored::EArtificialIntelligence)
			LaunchTheBallToAI();
		else
			LaunchTheBallToPlayer();
	}

}

void APongGameStateBase::InitLevelActors()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		// Spawn the ball into the world.
		TheBall = World->SpawnActor<ABall>(ABall::StaticClass(), BallStartLocation, FRotator::ZeroRotator);

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPaddle::StaticClass(), FoundActors);

		for (AActor* TActor : FoundActors)
		{
			AIPaddle = Cast<AAIPaddle>(TActor);

			if (AIPaddle != nullptr)
			{
				AIPaddle->SetGameBall(TheBall);
				break;
			}
		}
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaddle::StaticClass(), FoundActors);

		for (AActor* TActor : FoundActors)
		{
			PlayerPaddle = Cast<APaddle>(TActor);

			if (PlayerPaddle != nullptr)
				break;
		}
	}
}

void APongGameStateBase::LaunchTheBallToPlayer()
{
	LaunchTheBall(-1.0f);
}

void APongGameStateBase::LaunchTheBallToAI()
{
	LaunchTheBall(1.0f);
}

void APongGameStateBase::ResetTheBall()
{
	TheBall->StopMovement();

	TheBall->SetActorLocation(BallStartLocation);
}

void APongGameStateBase::LaunchTheBall(float YDirection)
{
	if (TheBall != nullptr)
	{
		FVector LaunchDirection(0.0f, YDirection, 0.0f);
		TheBall->FireInDirection(LaunchDirection);
	}
}
