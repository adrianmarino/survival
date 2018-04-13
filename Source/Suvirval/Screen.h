// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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
};
