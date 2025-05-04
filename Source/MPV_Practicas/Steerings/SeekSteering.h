#pragma once

#include "Steering.h"

class AAICharacter;

class SeekSteering : public SteeringBehaviour
{
public:
	SeekSteering(AAICharacter* InCharacter);
	
	virtual FSOutputSteering GetSteering(float DeltaTime) override;
private:
	AAICharacter* Character;
	//FVector Target;
};
