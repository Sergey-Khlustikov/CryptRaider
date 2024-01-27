// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetOwner()->GetActorLocation();
}

// Called every frame
void UMover::TickComponent(
	float const DeltaTime,
	ELevelTick const TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move(DeltaTime);
}

void UMover::Move(float const DeltaTime)
{
	FVector const CurrentLocation = GetOwner()->GetActorLocation();
	FVector const TargetLocation = ShouldMove ? StartLocation + MoveOffset : StartLocation;
	float const Speed = FVector::Dist(CurrentLocation, TargetLocation) / MoveTime;

	FVector const FinalLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);

	GetOwner()->SetActorLocation(FinalLocation);
}

void UMover::SetShouldMove(bool const NewShouldMove)
{
	ShouldMove = NewShouldMove;
}
