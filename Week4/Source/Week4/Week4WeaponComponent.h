// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Week4WeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFire);


class AWeek4Character;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WEEK4_API UWeek4WeaponComponent : public USkeletalMeshComponent {
  GENERATED_BODY()

public:
  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category=Projectile)
  TSubclassOf<class AWeek4Projectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
  USoundBase* FireSound;

  /** AnimMontage to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
  UAnimMontage* FireAnimation;

  /** Gun muzzle's offset from the characters location */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
  FVector MuzzleOffset;

  /** MappingContext */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
  class UInputMappingContext* FireMappingContext;

  /** Fire Input Action */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
  class UInputAction* FireAction;

  /** Sets default values for this component's properties */
  UWeek4WeaponComponent();

  virtual void BeginPlay() override;

  /** Attaches the actor to a FirstPersonCharacter */
  UFUNCTION(BlueprintCallable, Category="Weapon")
  bool AttachWeapon(AWeek4Character* TargetCharacter);

  /** Make the weapon Fire a Projectile */
  UFUNCTION(BlueprintCallable, Category="Weapon")
  void Fire();

  UPROPERTY(BlueprintAssignable)
  FOnFire OnFire;

  UPROPERTY(BlueprintReadWrite)
  int32 RemainingAmmo;

  UPROPERTY(BlueprintReadWrite)
  int32 MaxAmmo = 30;

protected:
  /** Ends gameplay for this component. */
  UFUNCTION()
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
  /** The Character holding this weapon*/
  AWeek4Character* Character;
};