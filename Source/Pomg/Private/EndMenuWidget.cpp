// Fill out your copyright notice in the Description page of Project Settings.


#include "EndMenuWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "PongGameStateBase.h"

void UEndMenuWidget::NativeConstruct()
{
	Button_NewGame->OnClicked.AddUniqueDynamic(this, &UEndMenuWidget::NewGameClicked);
}

void UEndMenuWidget::SetEndMessage(FText Message)
{
	Text_EndMessage->SetText(Message);
}

void UEndMenuWidget::NewGameClicked()
{
	UWorld* const world = GetWorld();
	if (world)
	{
		APongGameStateBase* GameState = Cast<APongGameStateBase>(world->GetGameState());

		GameState->ResetGame();

		RemoveFromViewport();

		GameState->StartGame();
	}
}
