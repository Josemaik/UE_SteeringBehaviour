#include "C:\Users\JOSE MANUEL\Desktop\UTAD\IA\MPV_Practicas_Skeleton\MPV_Practicas_Skeleton\Intermediate\Build\Win64\x64\MPV_PracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.RTTI.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "SeekSteering.h"
#include "MPV_Practicas/AICharacter.h"

SeekSteering::SeekSteering(AAICharacter* InCharacter)
	:Character(InCharacter)
{
}

FSOutputSteering SeekSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering Output;

	if (!Character) return Output;

	//Get Target
	FVector Target = Character->GetParams().targetPosition;
	
	//Target Direction
	FVector Position = Character->GetActorLocation();
	FVector Direction = Target - Position;
	Direction.Normalize();
	
	//Desired velocity
	FVector DesiredVelocity = Direction * Character->GetParams().max_velocity;
	
	//Acceleration
	FVector CurrentVelocity = Character->GetCurrentVelocity();
	FVector Acceleration = (DesiredVelocity - CurrentVelocity) / DeltaTime;

	//clamp acceleration
	float MaxAcceleration = Character->GetParams().max_acceleration;
	if (Acceleration.Length() > MaxAcceleration)
	{
		Acceleration = Acceleration.GetClampedToMaxSize(MaxAcceleration);
	}
	
	Output.LinearAcceleration = Acceleration;
	Output.AngularAcceleration = 0.f;
	
	return Output;
}


