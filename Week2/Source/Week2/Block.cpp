// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"

#include "GameTracker.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABlock::ABlock() {
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you
  // don't need it.
  PrimaryActorTick.bCanEverTick = true;

  Tags.Add(FName{"Block"});
  SetReplicates(true);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay() {
  Super::BeginPlay();

  if (auto* Tracker = Cast<AGameTracker>(
          UGameplayStatics::GetActorOfClass(GetWorld(), AGameTracker::StaticClass()))) {
    MaxHitCount = Tracker->MaxHitCount;
    ScaleOnHit = Tracker->ScaleOnHit;
  }


  if (ScaleCurve) {
    ScaleTimeline.SetLooping(false);

    FOnTimelineFloat OnTimelineFloat{};
    OnTimelineFloat.BindUFunction(this, FName{"TimelineUpdate"});
    ScaleTimeline.AddInterpFloat(ScaleCurve, OnTimelineFloat);
  }
}

void ABlock::OnRep_IsSpecial() {
  ChangeColor(FLinearColor::Red);
}

void ABlock::TimelineUpdate(float Progress) {
  using UE::Math::TVector2;
  float Scale = FMath::GetMappedRangeValueClamped(TVector2{0.f, 1.f}, TVector2{ScaleFrom, ScaleTo},
                                                  Progress);
  SetActorScale3D({Scale, Scale, Scale});
}

void ABlock::UpdateScaleRange() {
  ScaleFrom = GetActorScale3D().GetMin();
  ScaleTo = ScaleFrom * ScaleOnHit;
}

// Called every frame
void ABlock::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  ScaleTimeline.TickTimeline(DeltaTime);
}

void ABlock::OnHit() {
  ++HitCount;
  OnBlockHit.Broadcast(bIsSpecial);

  if (HitCount >= MaxHitCount) {
    OnBlockDestroyed.Broadcast(bIsSpecial);
    Destroy();
    return;
  }

  UpdateScaleRange();
  ScaleTimeline.PlayFromStart();
}

void ABlock::ChangeColor(FLinearColor NewColor) {
  if (auto* StaticMesh = Cast<UStaticMeshComponent>(GetRootComponent())) {
    if (auto* Mat = StaticMesh->CreateAndSetMaterialInstanceDynamic(0)) {
      Mat->SetVectorParameterValue(TEXT("Base Color"), NewColor);
    }
  }
}

void ABlock::MakeSpecial() {
  bIsSpecial = true;
}

void ABlock::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(ABlock, bIsSpecial);
}
