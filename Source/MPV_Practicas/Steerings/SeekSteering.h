#pragma once

#include "Steering.h"

class AAICharacter;

class SeekSteering : public SteeringBehaviour
{
public:
	SeekSteering(AAICharacter* InCharacter);
	
	virtual FSOutputSteering GetSteering(float DeltaTime) override;
	virtual void DrawDebug() override;
private:
	AAICharacter* Character;
	//FVector Target;
	FVector LastDesiredVelocity = FVector::ZeroVector;
	FVector LastAcceleration = FVector::ZeroVector;
};
