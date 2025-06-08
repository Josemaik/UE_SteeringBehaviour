#include "C:\Users\JOSE MANUEL\Desktop\UTAD\IA\MPV_Practicas_Skeleton\Practicas\UE_SteeringBehaviour\Intermediate\Build\Win64\x64\MPV_PracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.RTTI.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "ObstacleAvoidanceSteering.h"
#include "DrawDebugHelpers.h"
#include "../Structs/Obstacle.h"

ObstacleAvoidanceSteering::ObstacleAvoidanceSteering(AAICharacter* character, const TArray<Obstacle>& obstacles)
	: m_character(character), m_obstacles(obstacles)
{
}

FSOutputSteering ObstacleAvoidanceSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering output;

	FVector pos = m_character->GetActorLocation();
	FVector vel = m_character->GetCurrentVelocity();

	if (vel.IsNearlyZero())
		return output;

	FVector dir = vel.GetSafeNormal();
	float lookAhead = m_character->GetParams().look_ahead;
	float charRadius = m_character->GetParams().char_radius;
	const float avoidanceStrength = m_character->GetParams().max_acceleration * 25.5f;
	
	float closestDist = FLT_MAX;
	FVector avoidanceForce = FVector::ZeroVector;
	bool found = false;

	for (const Obstacle& obs : m_obstacles)
	{
		FVector toObstacle = obs.position - m_character->GetActorLocation();
		float forwardDist = FVector::DotProduct(toObstacle, dir);

		if (forwardDist < 0)
			continue;

		//FVector projected = dir * forwardDist;
		FVector lateralVec = toObstacle;// - projected;
		float lateralDist = lateralVec.Size();

		float combinedRadius = obs.radius + charRadius;
		
		DrawDebugCircle(
			m_character->GetWorld(),
			obs.position,
			combinedRadius,
			32,
			FColor::Green,
			false,
			0.1f,
			0,
			2.0f,
			FVector(1, 0, 0), // eje X
			FVector(0, 0, 1), // eje Y
			false // no dibujar en 3D
		);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Lateraldist: %f Combined: %f"),
			lateralDist,combinedRadius));
		
		if (lateralDist < combinedRadius)
		{
			//if (forwardDist < closestDist)
			//{
				// Dirección de evasión lateral respecto al movimiento
				FVector avoidanceDir = FVector::CrossProduct(dir, FVector::RightVector); // Perpendicular a dir en el plano horizontal

		
				// Dirección de evasión: lateral respecto al obstáculo
				//FVector avoidanceDir = lateralVec.GetSafeNormal();
				// Decidir dirección según de qué lado está el obstáculo
				if (FVector::DotProduct(avoidanceDir, lateralVec) < 0)
				{
					avoidanceDir *= -1.f; //asi funciona pero colapsa con el obstaculo 
				}						  //Sin esto esquiva el obstaculo a trompicines
				// Escalar según cercanía y penetración
				float penetration = combinedRadius - lateralDist;
				avoidanceForce = avoidanceDir * (avoidanceStrength + penetration);

				closestDist = forwardDist;
				found = true;
			//}
		}
	}

	if (found)
	{
		output.LinearAcceleration = avoidanceForce; //GetClampedToMaxSize(m_character->GetParams().max_acceleration * 2.0f);

		// Debug
		DrawDebugLine(m_character->GetWorld(), pos, pos + output.LinearAcceleration * 0.2f, FColor::Red, false, 0.1f, 0, 4.f);
	}

	return output;
}

void ObstacleAvoidanceSteering::DrawDebug()
{
}
