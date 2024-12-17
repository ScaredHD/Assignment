// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameTracker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimesUp);

UCLASS()
class WEEK2_API AGameTracker : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AGameTracker();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable)
  void UpdateScore(bool IsBlockSpecial);

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 MaxHitCount{2};

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 ScoreOnHit{2};

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  uint8 BonusOnHit{3};

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float ScaleOnHit{1.5f};

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  int32 InitialSpecialBlockCount{5};

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  int32 PlayerDestroyedBlockCount{};

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  int32 PlayerScore{};

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  TArray<AActor*> Blocks;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float GameTime = 10.f;

  FTimerHandle TimerHandle;
  
  FTimerHandle CounterHandle;

  UPROPERTY(BlueprintAssignable)
  FOnTimesUp OnTimesUp;

  void OnTimerTriggered();

  void OnCounterTimerTriggered();
};
