#include "C:\Users\JOSE MANUEL\Desktop\UTAD\IA\MPV_Practicas_Skeleton\UE_SteeringBehaviour\Intermediate\Build\Win64\x64\MPV_PracticasEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.RTTI.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "ArriveSteering.h"
#include "MPV_Practicas/AICharacter.h"
#include "MPV_Practicas/debug/debugdraw.h"

FSOutputSteering ArriveSteering::GetSteering(float DeltaTime)
{
	FSOutputSteering Output;

	if (!Character) return Output;

	//Get Target
	FVector Target = Character->GetParams().targetPosition;
	//Current Pos
	FVector Position = Character->GetActorLocation();
	
	//Target Direction
	FVector Direction = Target - Position;
	Direction.Normalize();

	float distance = FVector::Dist(Position, Target);
	float arriveRadius = Character->GetParams().deceleration_radius;

	FVector DesiredVelocity;
	if (distance > arriveRadius)
	{
		// Max speed
		DesiredVelocity = Direction * Character->GetParams().max_velocity;
	}
	else
	{
		// Inside radius - lerp velocity
		float scaledSpeed = Character->GetParams().max_velocity * (distance / arriveRadius);
		DesiredVelocity = Direction * scaledSpeed;
	}
	
	FVector Acceleration = (DesiredVelocity - Character->GetCurrentVelocity()) / DeltaTime;
	//clamp acceleration
	float MaxAcceleration = Character->GetParams().max_acceleration;
	if (Acceleration.Length() > MaxAcceleration)
	{
		Acceleration = Acceleration.GetClampedToMaxSize(MaxAcceleration);
	}
	
	Output.LinearAcceleration = Acceleration;
	Output.AngularAcceleration = 0.f;

	// Save for debug
	LastDesiredVelocity = DesiredVelocity;
	LastAcceleration = Acceleration;
	
	return Output;
}

void ArriveSteering::DrawDebug()
{
	if (!Character) return;

	FVector Position = Character->GetActorLocation();

	// Desired Velocity
	if (!LastDesiredVelocity.IsNearlyZero())
	{
		FVector dir = LastDesiredVelocity.GetSafeNormal();
		float length = LastDesiredVelocity.Size();
		SetArrow(Character, TEXT("linear_velocity"), dir, length);
	}
	// Acceleration
	if (!LastAcceleration.IsNearlyZero())
	{
		FVector dir = LastAcceleration.GetSafeNormal();
		float length = LastAcceleration.Size();
		SetArrow(Character, TEXT("linear_acceleration"), dir, length);
	}
}
