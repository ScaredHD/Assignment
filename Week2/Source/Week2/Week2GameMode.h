// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Week2GameMode.generated.h"

UCLASS(minimalapi)

class AWeek2GameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  AWeek2GameMode();

  virtual void BeginPlay() override;

  UFUNCTION(BlueprintGetter)
  uint8 GetMaxHitCount() const { return MaxHitCount; }

protected:
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

};
