#pragma once

#include "Steering.h"

class AAICharacter;
class SeekSteering;

class PathFollowing : public SteeringBehaviour
{
public:
	PathFollowing(AAICharacter* character, const TArray<FVector>& inPath);
	virtual ~PathFollowing() override;

	virtual FSOutputSteering GetSteering(float DeltaTime) override;
	virtual void DrawDebug() override;

private:
	AAICharacter* m_character;
	TArray<FVector> m_path;
	int32 m_currentIndex;
	FVector m_target;

	SeekSteering* m_seek;

	bool bInsidePath = false;

	FVector GetClosestPointOnPath();
	int32 GetClosestIndexOnPath();
	void AdvanceToNextIfClose();
};
