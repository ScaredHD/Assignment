// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Week2PlayerState.generated.h"

/**
 *
 */
UCLASS()

class WEEK2_API AWeek2PlayerState : public APlayerState {
  GENERATED_BODY()


protected:
  uint32 DestroyedBlockCount{};
};
