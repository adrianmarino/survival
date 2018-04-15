#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damaging.generated.h"

UINTERFACE(Blueprintable)
class UDamaging : public UInterface { 
	GENERATED_BODY()
};

class SUVIRVAL_API IDamaging
{
	GENERATED_BODY()

	public:
		virtual bool LifeIsZero() { return true; };

		virtual void Damage(float Quantity) {};

		virtual void IncreaseArmor(float Quantity) {};

		virtual void IncreaseHealth(float Quantity) {};

		virtual bool ArmorIsFull() { return false; };

		virtual bool HealthIsFull() { return false; };

		virtual float GetCurrentArmor() { return 0; };

		virtual float GetCurrentHealth() { return 0; };
};
