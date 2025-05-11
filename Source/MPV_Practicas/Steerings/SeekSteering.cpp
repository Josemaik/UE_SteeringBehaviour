#include "SeekSteering.h"
#include "MPV_Practicas/AICharacter.h"
#include "MPV_Practicas/debug/debugdraw.h"

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
	//Current Pos
	FVector Position = Character->GetActorLocation();
	
	//If we want to stop--------------------------------
	//Check distance to target
	float Distance = FVector::Dist(Position, Target);
	if (Distance < Character->GetParams().dest_radius)
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green,
			FString::Printf(TEXT("Stop Movement")));
		// Stop movement
		LastDesiredVelocity = FVector::ZeroVector;
		LastAcceleration = FVector::ZeroVector;
		Output.LinearAcceleration = FVector::ZeroVector;
		Output.AngularAcceleration = 0.f;
		Output.stop = true;
		return Output;
	}
	//------------------------------------------------------
	
	//Target Direction
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

	// Save for debug
	LastDesiredVelocity = DesiredVelocity;
	LastAcceleration = Acceleration;
	
	return Output;
}

void SeekSteering::DrawDebug()
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


