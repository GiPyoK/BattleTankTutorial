// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Tank NOT found"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Player Tank: %s"), *(PlayerTank->GetName()));
}


ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn =  GetWorld()->GetFirstPlayerController();
	if (!PlayerPawn)
	{
		return nullptr;
	}

	return Cast<ATank>(PlayerPawn->GetPawn());
}


