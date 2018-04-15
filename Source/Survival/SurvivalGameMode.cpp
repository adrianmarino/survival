#include "SurvivalGameMode.h"
#include "SurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASurvivalGameMode::ASurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL) {
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
