// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts`
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (!PhysicsHandle || !PhysicsHandle->GetGrabbedComponent())
	{
		return;
	}

	FVector const NewLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

	PhysicsHandle->SetTargetLocationAndRotation(NewLocation, GetComponentRotation());
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (!PhysicsHandle)
	{
		return;
	}

	if (FHitResult HitResult; GetFirstPhysicsBodyInReach(HitResult))
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		AActor* HitActor = HitResult.GetActor();
		
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (!PhysicsHandle || !PhysicsHandle->GetGrabbedComponent())
	{
		return;
	}

	PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
	PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
	PhysicsHandle->ReleaseComponent();
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found on %s"), *GetOwner()->GetName());
	}

	return PhysicsHandle;
}

void UGrabber::MoveCloserOrFarther(float const Delta)
{
	// Do not allow the player to move the object closer than 0.0f or farther than MaxGrabDistance
	if (HoldDistance + Delta < 0.0f || HoldDistance + Delta > MaxGrabDistance)
	{
		return;
	}

	HoldDistance += Delta * 10.0f;
}

bool UGrabber::GetFirstPhysicsBodyInReach(FHitResult& HitResult) const
{
	FVector const Start = GetComponentLocation();
	FVector const End = Start + GetForwardVector() * MaxGrabDistance;
	FCollisionShape const Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);
}
