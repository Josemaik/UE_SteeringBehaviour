#pragma once

#include "Steering.h"
#include "MPV_Practicas/AICharacter.h"

struct Obstacle;

class ObstacleAvoidanceSteering : public SteeringBehaviour
{
public:
	ObstacleAvoidanceSteering(AAICharacter* character, const TArray<Obstacle>& obstacles);
	virtual FSOutputSteering GetSteering(float DeltaTime) override;
	virtual void DrawDebug() override;
private:
	AAICharacter* m_character;
	const TArray<Obstacle>& m_obstacles;
};
