// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"

#include "Week2WeaponComponent.generated.h"

class AWeek2Character;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class WEEK2_API UWeek2WeaponComponent : public USkeletalMeshComponent {
  GENERATED_BODY()

public:
  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = Projectile)
  TSubclassOf<class AWeek2Projectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  USoundBase* FireSound;

  /** AnimMontage to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  UAnimMontage* FireAnimation;

  /** Gun muzzle's offset from the characters location */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  FVector MuzzleOffset;

  /** MappingContext */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputMappingContext* FireMappingContext;

  /** Fire Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
  class UInputAction* FireAction;

  /** Sets default values for this component's properties */
  UWeek2WeaponComponent();

  /** Attaches the actor to a FirstPersonCharacter */
  UFUNCTION(BlueprintCallable, Category = "Weapon")
  bool AttachWeapon(AWeek2Character* TargetCharacter);

  /** Make the weapon Fire a Projectile */
  UFUNCTION(BlueprintCallable, Category = "Weapon")
  void Fire();

  UFUNCTION(Server, Reliable)
  void Server_Fire();

protected:
  /** Ends gameplay for this component. */
  UFUNCTION()
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
  /** The Character holding this weapon*/
  AWeek2Character* Character;
};
