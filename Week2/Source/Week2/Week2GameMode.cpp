// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week2GameMode.h"

#include "Block.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Week2Character.h"
#include "Week2GameState.h"

AWeek2GameMode::AWeek2GameMode() : Super() {
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
      TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
  DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AWeek2GameMode::BeginPlay() {
  Super::BeginPlay();

  TArray<AActor*> Blocks;
  UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName{"Block"}, Blocks);
  int32 RemainBlockCount = Blocks.Num();
  int32 RemainSpecialCount = FMath::Min(InitialSpecialBlockCount, RemainBlockCount);
  for (auto& Actor : Blocks) {
    if (auto* Block = Cast<ABlock>(Actor)) {
      Block->ScaleOnHit = ScaleOnHit;
      Block->MaxHitCount = MaxHitCount;

      // Block->OnBlockHit.AddDynamic(this, &AGameTracker::UpdateScore);

      // Randomly make some blocks special (selection sampling)
      float Prob = static_cast<float>(RemainSpecialCount) / RemainBlockCount;
      if (auto R = FMath::FRandRange(0.f, 1.f); R < Prob) {
        Block->MakeSpecial();
        Block->ChangeColor(FLinearColor::Red);
        --RemainSpecialCount;
      }

      --RemainBlockCount;
    }
  }
}
