// Fill out your copyright notice in the Description page of Project Settings.


#include "GameTracker.h"

#include "Block.h"
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeek2Game, Log, Log);
DEFINE_LOG_CATEGORY(LogWeek2Game);

// Sets default values
AGameTracker::AGameTracker() {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameTracker::BeginPlay() {
  Super::BeginPlay();

  UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName{"Block"}, Blocks);


  int32 RemainBlockCount = Blocks.Num();
  int32 RemainSpecialCount = std::min(InitialSpecialBlockCount, RemainBlockCount);
  for (auto& Actor : Blocks) {
    if (auto* Block = Cast<ABlock>(Actor)) {
      Block->OnBlockHit.AddDynamic(this, &AGameTracker::UpdateScore);

      // Randomly make some blocks special (selection sampling)
      float Prob = static_cast<float>(RemainSpecialCount) / RemainBlockCount;
      if (auto R = FMath::FRandRange(0.f, 1.f); R < Prob) {
        Block->MakeSpecial();
        --RemainSpecialCount;
      }

      --RemainBlockCount;
    }
  }


  auto& WorldTimerManager = GetWorldTimerManager();
  WorldTimerManager.SetTimer(TimerHandle, this, &AGameTracker::OnTimerTriggered, GameTime, false);
  WorldTimerManager.SetTimer(CounterHandle, this, &AGameTracker::OnCounterTimerTriggered, 1.f,
                             true);
}

// Called every frame
void AGameTracker::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);
}

void AGameTracker::UpdateScore(bool IsBlockSpecial) {
  ++PlayerDestroyedBlockCount;

  PlayerScore += ScoreOnHit;
  if (IsBlockSpecial) {
    PlayerScore += BonusOnHit;
  }
}

void AGameTracker::OnCounterTimerTriggered() {
  int TimerRemaining = FMath::Floor(GetWorldTimerManager().GetTimerRemaining(TimerHandle)) + 1;
  GEngine->AddOnScreenDebugMessage(-1, 0.9f, FColor::Red, FString::Printf(TEXT("%d"), TimerRemaining));
}


void AGameTracker::OnTimerTriggered() {
  OnTimesUp.Broadcast();
  UE_LOG(LogWeek2Game, Log, TEXT("Time's up. Player score: %d"), PlayerScore);
}