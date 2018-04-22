#pragma once

#include "functional"

class PercentLevel
{
	//-----------------------------------------------------------------------------
	// Constants
	//-----------------------------------------------------------------------------

	private:
		const float MIN_VALUE = 0.0f;
		const float MAX_VALUE = 1.0f;
		const float DEFAULT_STEP = 0.002f;

	//-----------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------

		float Value;

		float Step;

		std::function<bool()> IncreaseCondition;

		std::function<bool()> DecreaseCondition;

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------

	public:
		PercentLevel(float value);
		PercentLevel(float value, float step);

	//-----------------------------------------------------------------------------
	// Methods
	//-----------------------------------------------------------------------------

		void SetIncreaseCondition(std::function<bool()> Condition);
		void SetDecreaseCondition(std::function<bool()> Condition);

		float GetValue();

		void Increase();
		void ForceIncrease();
		void Increase(float step);
		void ForceIncrease(float step);

		void Decrease();
		void Decrease(float step);

		bool IsFull();
		bool IsZero();

	private:
		void SetValue(float value);
};
