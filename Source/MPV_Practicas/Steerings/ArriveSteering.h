#pragma once

#include "Steering.h"

class AAICharacter;

class ArriveSteering : public SteeringBehaviour
{
public:
	ArriveSteering(AAICharacter* InCharacter) :
		Character(InCharacter){};
	
	virtual FSOutputSteering GetSteering(float DeltaTime) override;
	virtual void DrawDebug() override;
private:
	AAICharacter* Character;
	FVector LastDesiredVelocity = FVector::ZeroVector;
	FVector LastAcceleration = FVector::ZeroVector;
};
