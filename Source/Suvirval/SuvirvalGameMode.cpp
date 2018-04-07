// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SuvirvalGameMode.h"
#include "SuvirvalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASuvirvalGameMode::ASuvirvalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
