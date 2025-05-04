#pragma once

#include "CoreMinimal.h"
#include "FSOutputSteering.h"

class SteeringBehaviour
{
public:
	virtual ~SteeringBehaviour() {}
	// Pure Method
	virtual FSOutputSteering GetSteering(float DeltaTime) = 0;
	virtual void DrawDebug() = 0;
};