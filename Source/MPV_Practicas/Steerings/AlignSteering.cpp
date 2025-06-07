#include "AlignSteering.h"
#include "MPV_Practicas/AICharacter.h"
#include "MPV_Practicas/debug/debugdraw.h"

FSOutputSteering AlignSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering Output;
	
	if (!Character) return Output;

	// Parámetros
	const float maxAngularSpeed = Character->GetParams().max_angular_velocity;
	const float maxAngularAcc = Character->GetParams().max_angular_acceleration;
	const float angularArriveRadius = Character->GetParams().angular_arrive_radius;
	//const float timeToTarget = 0.1f; // Ajustable para suavizar

	float TargetOrientation = Character->GetParams().targetRotation;
	float CurrentOrientation = Character->GetOrientation();

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target Orientation: %f"), TargetOrientation));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Current Orientation: %f"), CurrentOrientation));	
	//
	//desired angular velocity
	// Asegura que ambas orientaciones están en grados (usa radianes si prefieres)
	float Rotation = FMath::UnwindDegrees(TargetOrientation - CurrentOrientation);
	float RotationSize = FMath::Abs(Rotation);
	//float Rotation = TargetOrientation - CurrentOrientation;
	
	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("Rotation: %f"), Rotation));

	//desired angular acceleration
	//float targetAngularSpeed = Character->GetParams().max_angular_velocity;
	//float RotationSize = FMath::Abs(Rotation);
	
	// Si estás dentro del radio de llegada, escala la velocidad
	// if (RotationSize < Character->GetParams().angular_arrive_radius)
	// {
	// 	targetAngularSpeed *= RotationSize / Character->GetParams().angular_arrive_radius;
	// }
	
	//targetAngularSpeed *= FMath::Sign(Rotation); // dirección
	if (RotationSize < 0.5f) {
		Output.AngularAcceleration = -Character->GetCurretAngularVelocity() / DeltaTime;
		return Output;
	}
	// range [-π, π]
	//Rotation = FMath::UnwindRadians(Rotation);
	// Calcula velocidad angular objetivo
	float targetAngularSpeed = maxAngularSpeed;

	if (RotationSize < angularArriveRadius)
	{
		targetAngularSpeed *= RotationSize / angularArriveRadius; // suaviza
	}

	targetAngularSpeed *= FMath::Sign(Rotation); // dirección de giro

	float AngularAcceleration = (targetAngularSpeed - Character->GetCurretAngularVelocity()) / timeToTarget;
	
	Output.AngularAcceleration = FMath::Clamp(AngularAcceleration, -maxAngularAcc,maxAngularAcc);

	return Output;
}

void AlignSteering::DrawDebug()
{
	// Flecha del targetRotation
	float angle = FMath::DegreesToRadians(Character->GetParams().targetRotation);
	FVector dir = FVector(FMath::Cos(angle), 0.f, FMath::Sin(angle)); // XZ plano
	
	float arrowLength = 100.0f;
	SetArrow(Character, TEXT("targetRotation"), dir, arrowLength);
}
