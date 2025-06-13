//#include "C:\Users\JOSE MANUEL\Desktop\UTAD\IA\MPV_Practicas_Skeleton\Practicas\UE_SteeringBehaviour\Intermediate\Build\Win64\x64\MPV_PracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.RTTI.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
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

	FVector CharacterPosition = m_character->GetActorLocation();
	FVector vel = m_character->GetCurrentVelocity();
	float LookAhead = m_character->GetParams().look_ahead;
	float charRadius = m_character->GetParams().char_radius;
	float MaxAcceleration =	m_character->GetParams().max_acceleration;

	if (vel.IsNearlyZero())
		return output;

	FVector Ro = vel.GetSafeNormal();
	FVector LookAheadDirection = Ro * LookAhead;
	//const float avoidanceStrength =  MaxAcceleration * 25.5f;

	float closestDist = FLT_MAX;
	FVector bestAvoidance = FVector::ZeroVector;

	for (const Obstacle& obs : m_obstacles)
	{
		//const Obstacle& obs = m_obstacles[0];
		FVector ToObstacle = obs.position - CharacterPosition;
	
		float forwardDist = FVector::DotProduct(ToObstacle, Ro);
		if (forwardDist < 0 ||forwardDist > LookAheadDirection.Length())
		{
			continue;//return output;
		}
	
		FVector ClosestPoint = CharacterPosition + Ro * forwardDist;
		
	
		//continue;
		FVector Diff = ClosestPoint - obs.position;
		float dist = Diff.Length();

		if (dist >= obs.radius + charRadius) continue;//return output;

		FVector lp = Diff.GetSafeNormal();  // Vector desde obstáculo hacia punto más cercano

		// Producto cruzado en plano XZ → usamos componente Y
		float side = FVector::CrossProduct(Ro, lp).Y;

		// Vector perpendicular a 'ro' en XZ (rotado 90°)
		FVector avoidanceDir = FVector(-Ro.Z, 0.0f, Ro.X); // rotado a la izquierda

		if (side > 0) {
			// Si el obstáculo está a la derecha, giramos a la izquierda
			avoidanceDir *= -1.0f;
		}

		// Escalar fuerza según cercanía y penetración
		float penetration = obs.radius - dist;
		float strength = MaxAcceleration * (1.0f - forwardDist / LookAhead) + penetration * 10.0f;

		/*output.LinearAcceleration*/ FVector avoidanceForce = avoidanceDir.GetSafeNormal() * strength;
		//output.LinearAcceleration = output.LinearAcceleration.GetClampedToMaxSize(MaxAcceleration);

		// Escoger el obstáculo más urgente (más cercano en el eje de movimiento)
		if (forwardDist < closestDist)
		{
			closestDist = forwardDist;
			bestAvoidance = avoidanceForce;
		}
		
		//DrawDebugLine(m_character->GetWorld(), CharacterPosition, CharacterPosition + output.LinearAcceleration * 0.2f, FColor::Red, false, 0.1f, 0, 4.0f);
		
		//return output; 
	}

	if (!bestAvoidance.IsNearlyZero())
	{
		output.LinearAcceleration = bestAvoidance;
		DrawDebugLine(m_character->GetWorld(), CharacterPosition, CharacterPosition + output.LinearAcceleration * 0.2f, FColor::Red, false, 0.1f, 0, 4.0f);
	}
	
	return output;
}

void ObstacleAvoidanceSteering::DrawDebug()
{
}
