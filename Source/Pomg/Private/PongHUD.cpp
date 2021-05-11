// Fill out your copyright notice in the Description page of Project Settings.


#include "PongHUD.h"
#include "PongGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Canvas.h"

const float MidScreenOffset = 80.0f;

APongHUD::APongHUD()
{
	ConstructorHelpers::FObjectFinder<UFont> ScoreFontObject(TEXT("Font'/Game/Fonts/ScoreFont.ScoreFont'"));
	if (ScoreFontObject.Object)
		ScoreFont = ScoreFontObject.Object;
	else
		ScoreFont = nullptr;

	ConstructorHelpers::FObjectFinder<UFont> TextFontObject(TEXT("Font'/Game/Fonts/TextFont.TextFont'"));
	if (TextFontObject.Object)
		TextFont = TextFontObject.Object;
	else
		TextFont = nullptr;

	PlayerScore = 0;
	AIScore = 0;
}

void APongHUD::DrawHUD()
{
	// Call the parent's version of DrawHUD.
	Super::DrawHUD();

	//FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	//FVector2D PlayerScoreTextSize;
	//FVector2D AIScoreTextSize;
	//FVector2D StartTextSize;

	//FString PlayerScoreString;
	//FString AIScoreString;

	//float TextScale = 1.0f;

	//// Player's and AI's score string.
	//PlayerScoreString = FString::FromInt(PlayerScore);
	//AIScoreString = FString::FromInt(AIScore);

	//// Player's score text size.
	//GetTextSize(PlayerScoreString, PlayerScoreTextSize.X, PlayerScoreTextSize.Y, ScoreFont);
	//// AI's score text size.
	//GetTextSize(AIScoreString, AIScoreTextSize.X, AIScoreTextSize.Y, ScoreFont);

	//// Draw score.
	//FVector2D sPlayerLoc;
	//GetOwningPlayerController()->ProjectWorldLocationToScreen(FVector(530.0f, -600.0f, 950.0f), sPlayerLoc);

	//FVector2D sAILoc;
	//GetOwningPlayerController()->ProjectWorldLocationToScreen(FVector(530.0f, -20.0f, 950.0f), sAILoc);

	//DrawText(PlayerScoreString, FColor::White, sPlayerLoc.X, sPlayerLoc.Y, TextFont, TextScale);
	//DrawText(AIScoreString, FColor::White, sAILoc.X, sAILoc.Y, TextFont, TextScale);

	//APongGameStateBase* GameState = Cast<APongGameStateBase>(UGameplayStatics::GetGameState(this));

	//if (GameState->GetCurrentState() == EPongStates::EWaitingToStart)
	//{
	//	GetTextSize(TEXT("PLAY"), StartTextSize.X, StartTextSize.Y, TextFont);
	//	DrawText(TEXT("PLAY"), FColor::White, (ScreenDimensions.X - StartTextSize.X) / 2.0f, (ScreenDimensions.Y - StartTextSize.Y) / 2.0f, TextFont, TextScale);
	//}

}

void APongHUD::SetAIScored()
{
	AIScore++;
}

void APongHUD::SetPlayerScored()
{
	PlayerScore++;
}
