// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week4PickUpComponent.h"

UWeek4PickUpComponent::UWeek4PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UWeek4PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UWeek4PickUpComponent::OnSphereBeginOverlap);
}

void UWeek4PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AWeek4Character* Character = Cast<AWeek4Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
