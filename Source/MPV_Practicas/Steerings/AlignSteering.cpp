#include "C:\Users\JOSE MANUEL\Desktop\UTAD\IA\MPV_Practicas_Skeleton\UE_SteeringBehaviour\Intermediate\Build\Win64\x64\MPV_PracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.RTTI.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "AlignSteering.h"
#include "MPV_Practicas/AICharacter.h"
#include "MPV_Practicas/debug/debugdraw.h"

FSOutputSteering AlignSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering Output;
	
	float TargetOrientation = Character->GetParams().targetRotation;
	float CurrentOrientation = Character->GetOrientation();

	float Rotation = TargetOrientation - CurrentOrientation;
	// range [-π, π]
	Rotation = FMath::UnwindRadians(Rotation);

	//rotation size
	float RotationSize = FMath::Abs(Rotation);

	// Si ya está suficientemente cerca, no giramos
	if (RotationSize < Character->GetParams().align_radius)
	{
		Output.AngularAcceleration = 0.f;
		Output.stop = true;
		return Output;
	}

	// Calculamos velocidad deseada
	float targetAngularSpeed = Character->GetParams().max_angular_velocity;

	// Si estamos dentro de la zona de llegada, interpolamos la velocidad
	float angular_arrive_radius = Character->GetParams().angular_arrive_radius;
	if (RotationSize < angular_arrive_radius)
	{
		targetAngularSpeed *= RotationSize / angular_arrive_radius;
	}

	// La dirección del giro
	targetAngularSpeed *= FMath::Sign(Rotation);

	// Aceleración deseada
	float AngularAcceleration = (targetAngularSpeed - Character->GetCurretAngularVelocity()) / timeToTarget;
	
	// Clamp a aceleración máxima
	AngularAcceleration = FMath::Clamp(AngularAcceleration, -Character->GetParams().max_angular_acceleration, Character->GetParams().max_angular_acceleration);
	
	Output.AngularAcceleration = AngularAcceleration;
	return Output;
}

void AlignSteering::DrawDebug()
{
	// Flecha del targetRotation (en amarillo, por ejemplo)
	float angle = Character->GetParams().targetRotation; // en radianes
	FVector2D dir2D = FVector2D(FMath::Cos(angle), FMath::Sin(angle));
	FVector dir = FVector(dir2D.X, dir2D.Y, 0.f); // convertir a 3D

	float arrowLength = 100.0f; // Puedes ajustar la longitud a tu gusto

	SetArrow(Character, TEXT("targetRotation"), dir, arrowLength);
}
