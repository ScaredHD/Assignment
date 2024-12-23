// Copyright Epic Games, Inc. All Rights Reserved.

#include "Week4GameMode.h"
#include "Week4Character.h"
#include "UObject/ConstructorHelpers.h"

AWeek4GameMode::AWeek4GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}