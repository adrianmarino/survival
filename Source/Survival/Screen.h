#pragma once

#include "CoreMinimal.h"
#include "PercentLevel.h"

class Screen
{
	private:
		Screen();

	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

	public:

		static void Message(const FString& message) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, message);
		}

		static void ShowDamage(PercentLevel* Health, PercentLevel* Armor) {
				Screen::Message(FString::Printf(TEXT("Health: %1.3f - Full: %d"), Health->GetValue(), Health->IsFull()));
				Screen::Message(FString::Printf(TEXT("Armor: %1.3f - Full: %d"), Armor->GetValue(), Armor->IsFull()));
		}
};
