#pragma once

#include "Steering.h"

class AAICharacter;

class AlignSteering : public SteeringBehaviour
{
public:
	AlignSteering(AAICharacter* InCharacter) :
		Character(InCharacter){};
	
	virtual FSOutputSteering GetSteering(float DeltaTime) override;
	virtual void DrawDebug() override;
private:
	AAICharacter* Character;
	FVector LastOrientation = FVector::ZeroVector;

	float timeToTarget = 0.1f;
};
