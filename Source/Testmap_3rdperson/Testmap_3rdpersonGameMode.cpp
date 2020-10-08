// Copyright Epic Games, Inc. All Rights Reserved.

#include "Testmap_3rdpersonGameMode.h"
#include "Testmap_3rdpersonPlayerController.h"
#include "Testmap_3rdpersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestmap_3rdpersonGameMode::ATestmap_3rdpersonGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATestmap_3rdpersonPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}