// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Block.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockDestroyed, bool, IsSpecial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlockHit, bool, IsSpecial);

class UCurveFloat;

UCLASS()

class WEEK2_API ABlock : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ABlock();

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void OnHit();


  UFUNCTION(BlueprintCallable)
  void MakeSpecial();

  void ChangeColor(FLinearColor NewColor);


  virtual void GetLifetimeReplicatedProps(
      TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  uint8 HitCount{};

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  uint8 MaxHitCount{2};

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float ScaleOnHit{1.5f};

  FOnBlockDestroyed OnBlockDestroyed;

  FOnBlockHit OnBlockHit;

  UPROPERTY(EditAnywhere)
  UCurveFloat* ScaleCurve{};

  FTimeline ScaleTimeline{};

  UFUNCTION()
  void OnRep_IsSpecial();


protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;


private:
  UPROPERTY(ReplicatedUsing = OnRep_IsSpecial)
  bool bIsSpecial{false};

  UFUNCTION()
  void TimelineUpdate(float Progress);

  UFUNCTION()
  void UpdateScaleRange();


  float ScaleFrom{};

  float ScaleTo{};
};
