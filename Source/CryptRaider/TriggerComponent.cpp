// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(
	float const DeltaTime,
	ELevelTick const TickType,
	FActorComponentTickFunction* ThisTickFunction
)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* const TriggeredActor = GetFirstTriggeredActor();

	if (TriggeredActor != nullptr)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(TriggeredActor->GetRootComponent());

		if (Component != nullptr)
		{
			TriggeredActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
			Component->SetSimulatePhysics(false);
		}
		
		return Mover->SetShouldMove(true);
	}

	Mover->SetShouldMove(false);
}

AActor* UTriggerComponent::GetFirstTriggeredActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if (Actor->ActorHasTag(ActorTriggerTag) && !Actor->ActorHasTag("Grabbed"))
		{
			return Actor;
		}
	}

	return nullptr;
}

void UTriggerComponent::SetMover(UMover* const NewMover)
{
	Mover = NewMover;
}