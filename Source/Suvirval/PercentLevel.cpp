#include "PercentLevel.h"

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

PercentLevel::PercentLevel(float value) {
	this->SetValue(value);
	this->Step = DEFAULT_STEP;
	this->SetIncreaseCondition([]() { return true; });
	this->SetDecreaseCondition([]() { return true; });
}

//-----------------------------------------------------------------------------
// Methods
//-----------------------------------------------------------------------------

void PercentLevel::SetIncreaseCondition(std::function<bool()> condition) { this->IncreaseCondition = condition; }

void PercentLevel::SetDecreaseCondition(std::function<bool()> condition) { this->DecreaseCondition = condition; }

void PercentLevel::SetValue(float value) {
	if(value <= MIN_VALUE)
		this->Value = MIN_VALUE;
	else if(value >= MAX_VALUE)
		this->Value = MAX_VALUE;
	else
		this->Value = value;
}

float PercentLevel::GetValue() { return this->Value; }

void PercentLevel::Increase(float step) {
	if(IncreaseCondition()) {
		this->SetValue(Value + step);
	}
}

void PercentLevel::Increase() { this->Increase(DEFAULT_STEP); }

void PercentLevel::Drecrease(float step) { 
	if(DecreaseCondition()) {
		this->SetValue(Value - step);
	}
}

void PercentLevel::Drecrease() { this->Drecrease(DEFAULT_STEP); }

bool PercentLevel::IsFull() { return this->Value >= MAX_VALUE; }

bool PercentLevel::IsZero() { return this->Value <= MIN_VALUE; }