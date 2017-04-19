// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // TODO should this tick?

	// ...
}


void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel || !Turret)
	{
		return;
	}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("BarrelEndPoint"));

	// Calculate the OutLaunchVelocity
	bool bHanveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam
	);

	if (bHanveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveTankAimTowards(AimDirection);

		auto TankName = GetOwner()->GetName();
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: %s Aim solve found towards %s"), Time, *TankName, *AimDirection.ToString());
	}
	else
	{
		auto TankName = GetOwner()->GetName();
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("Aim solve NOT found"), Time, *TankName, *OutLaunchVelocity.ToString());
	}
}

void UTankAimingComponent::MoveTankAimTowards(FVector AimDirection)
{
	// Work out difference between current barrel rotation, and AimDirection
	auto BarrelRotation = Barrel->GetForwardVector().Rotation(); // roll, pitch, yaw
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaBarrelRotator = AimAsRotator - BarrelRotation;

	Barrel->Elevate(DeltaBarrelRotator.Pitch);

	auto TurretRotation = Turret->GetForwardVector().Rotation();
	auto DeltaTurretRotator = AimAsRotator - TurretRotation;

	Turret->Rotate(DeltaTurretRotator.Yaw);
}



