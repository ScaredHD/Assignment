// Copyright Epic Games, Inc. All Rights Reserved.


#include "Week2WeaponComponent.h"

#include "Animation/AnimInstance.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Week2Character.h"
#include "Week2Projectile.h"

// Sets default values for this component's properties
UWeek2WeaponComponent::UWeek2WeaponComponent() {
  // Default offset from the character location for projectiles to spawn
  MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UWeek2WeaponComponent::Fire() {
  if (!GetOwner()->HasAuthority()) {
    Server_Fire();
  }
  
  if (Character == nullptr || Character->GetController() == nullptr) {
    return;
  }

  // Try and fire a projectile
  if (ProjectileClass != nullptr) {
    UWorld* const World = GetWorld();
    if (World != nullptr) {
      APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
      const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
      // MuzzleOffset is in camera space, so transform it to world space before offsetting from the
      // character location to find the final muzzle position
      const FVector SpawnLocation = GetOwner()->GetActorLocation() +
                                    SpawnRotation.RotateVector(MuzzleOffset);

      // Set Spawn Collision Handling Override
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride =
          ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      // Spawn the projectile at the muzzle
      World->SpawnActor<AWeek2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation,
                                          ActorSpawnParams);
    }
  }

  // Try and play the sound if specified
  if (FireSound != nullptr) {
    UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
  }

  // Try and play a firing animation if specified
  if (FireAnimation != nullptr) {
    // Get the animation object for the arms mesh
    UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
    if (AnimInstance != nullptr) {
      AnimInstance->Montage_Play(FireAnimation, 1.f);
    }
  }
}
void UWeek2WeaponComponent::Server_Fire_Implementation() {
  Fire();
  UE_LOG(LogTemp, Log, TEXT("Server_Fire_Implementation"));
}

bool UWeek2WeaponComponent::AttachWeapon(AWeek2Character* TargetCharacter) {
  Character = TargetCharacter;

  // Check that the character is valid, and has no weapon component yet
  if (Character == nullptr ||
      Character->GetInstanceComponents().FindItemByClass<UWeek2WeaponComponent>()) {
    return false;
  }

  // Attach the weapon to the First Person Character
  FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
  AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

  // Set up action bindings
  if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                PlayerController->GetLocalPlayer())) {
      // Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire
      // action when using touch input
      Subsystem->AddMappingContext(FireMappingContext, 1);
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(
            PlayerController->InputComponent)) {
      // Fire
      EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this,
                                         &UWeek2WeaponComponent::Fire);
    }
  }

  return true;
}

void UWeek2WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
  // ensure we have a character owner
  if (Character != nullptr) {
    // remove the input mapping context from the Player Controller
    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
      if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
              ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                  PlayerController->GetLocalPlayer())) {
        Subsystem->RemoveMappingContext(FireMappingContext);
      }
    }
  }

  // maintain the EndPlay call chain
  Super::EndPlay(EndPlayReason);
}