#include "AlignSteering.h"
#include "MPV_Practicas/AICharacter.h"
#include "MPV_Practicas/debug/debugdraw.h"

FSOutputSteering AlignSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering Output;
	
	float TargetOrientation = Character->GetParams().targetRotation;
	float CurrentOrientation = Character->GetOrientation();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Target Orientation: %f"), TargetOrientation));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Current Orientation: %f"), CurrentOrientation));	
	//
	// FVector DirectionToTarget = TargetPosition - Character->GetActorLocation();
	// DirectionToTarget.Z = 0.f; // ignoramos la altura si no importa
	// DirectionToTarget.Normalize();
	//
	// // Convertir la dirección a un ángulo en radianes (Yaw)
	// float TargetOrientation = FMath::Atan2(DirectionToTarget.Y, DirectionToTarget.X);

	//if (TargetOrientation < 0) TargetOrientation += 2*UE_PI;
	//desired angular velocity
	float Rotation = TargetOrientation - CurrentOrientation;
	
	// range [-π, π]
	Rotation = FMath::UnwindRadians(Rotation);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("Rotation: %f"), Rotation));

	//desired angular acceleration
	float targetAngularSpeed = Character->GetParams().max_angular_velocity;
	float RotationSize = FMath::Abs(Rotation);
	
	// Si estás dentro del radio de llegada, escala la velocidad
	if (RotationSize < Character->GetParams().angular_arrive_radius)
	{
		targetAngularSpeed *= RotationSize / Character->GetParams().angular_arrive_radius;
	}

	targetAngularSpeed *= FMath::Sign(Rotation); // dirección

	float AngularAcceleration = (targetAngularSpeed - Character->GetCurretAngularVelocity()) / timeToTarget;
	Output.AngularAcceleration = FMath::Clamp(AngularAcceleration, -Character->GetParams().max_angular_acceleration, Character->GetParams().max_angular_acceleration);
	//float AngularAcceleration = (targetAngularSpeed - Character->GetCurretAngularVelocity()) / timeToTarget;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Emerald, FString::Printf(TEXT("maxaceleration: %f"), Character->GetParams().max_angular_acceleration));	
	// Clamp a aceleración máxima
	//Output.AngularAcceleration = FMath::Clamp(Rotation/timeToTarget, -Character->GetParams().max_angular_acceleration, Character->GetParams().max_angular_acceleration);
	
	//Output.AngularAcceleration = AngularAcceleration;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Output.AngularAcceleration: %f"), AngularAcceleration));	
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
