// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week2PickUpComponent.h"

UWeek2PickUpComponent::UWeek2PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UWeek2PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UWeek2PickUpComponent::OnSphereBeginOverlap);


}

void UWeek2PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AWeek2Character* Character = Cast<AWeek2Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
