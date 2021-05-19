// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDWidget.h"
#include "Components/TextBlock.h"

void UPlayerHUDWidget::SetPlayerScore(int NewScore)
{
	Player_Score->SetText(FText::AsNumber(NewScore));
}

void UPlayerHUDWidget::SetAIScore(int NewScore)
{
	AI_Score->SetText(FText::AsNumber(NewScore));
}
