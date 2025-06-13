// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "params/params.h"
#include "debug/debugdraw.h"
#include "Steerings/SeekSteering.h"
#include "Steerings/ArriveSteering.h"
#include "Steerings/AlignSteering.h"
#include "Steerings/Steering.h"
#include <memory>

#include "Steerings/ArriveSteering.h"
#include "Steerings/PathFollowing.h"
#include "Steerings/ObstacleAvoidanceSteering.h"
#include "Structs/Obstacle.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	ReadParams("params.xml", m_params);

	// 1. Initialize velocity
	CurrentVelocity = FVector(100.f, 0.0f, 0.0f);
	//Seek
	//m_steeringBehaviour = new SeekSteering(this);
	//Arrive
	//m_steeringBehaviour = new ArriveSteering(this);
	//Align
	currentAngularVelocity = GetOrientation();
	//m_steeringBehaviour = new AlignSteering(this);
	//m_steeringBehaviour2 = new ArriveSteering(this);
	m_steeringBehaviour = new PathFollowing(this, m_params.PathPoints);
	m_obstacleAvoidance = new ObstacleAvoidanceSteering(this,m_params.Obstacles);
}

void AAICharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_steeringBehaviour)
	{
		delete m_steeringBehaviour;
		m_steeringBehaviour = nullptr;
	}
	if (m_steeringBehaviour2)
	{
		delete m_steeringBehaviour2;
		m_steeringBehaviour2 = nullptr;
	}
	if (m_obstacleAvoidance)
	{
		delete m_obstacleAvoidance;
		m_obstacleAvoidance = nullptr;
	}
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	current_angle = GetActorAngle();

	//Aply steering
	if (m_steeringBehaviour /*&& m_steeringBehaviour2*/)
	{
		//FSOutputSteering SteeringOutputAlign = m_steeringBehaviour->GetSteering(DeltaTime);
		//FSOutputSteering SteeringOutputArrive = m_steeringBehaviour2->GetSteering(DeltaTime);
		FSOutputSteering SteeringOutputPath = m_steeringBehaviour->GetSteering(DeltaTime);
		FSOutputSteering avoid = m_obstacleAvoidance->GetSteering(DeltaTime);

		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("EvadedForce: %f,%f,%f"),
		// 	avoid.LinearAcceleration.X, avoid.LinearAcceleration.Y, avoid.LinearAcceleration.Z));
		//FVector blended = SteeringOutputPath.LinearAcceleration + avoid.LinearAcceleration;
		FVector blended = SteeringOutputPath.LinearAcceleration + avoid.LinearAcceleration * 2.f;
		//blended = blended.GetClampedToMaxSize(m_params.max_acceleration);
		//Seek
		// if (!SteeringOutput2.stop)
		// {
		// 	CurrentVelocity += SteeringOutput2.LinearAcceleration * DeltaTime;
		//
		// 	// Clamp
		// 	if (CurrentVelocity.Length() > m_params.max_velocity)
		// 	{
		// 		CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(m_params.max_velocity);
		// 	}
		// }
		// else
		// {
		// 	CurrentVelocity = SteeringOutput2.LinearAcceleration;
		// }
		//Arrive
		// CurrentVelocity += SteeringOutputArrive.LinearAcceleration * DeltaTime;
		//  //Clamp
		//  if (CurrentVelocity.Length() > m_params.max_velocity)
		//  {
		//  	CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(m_params.max_velocity);
		//  }
		// //Update position
		// FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
		// SetActorLocation(NewLocation);

		//Align
		// currentAngularVelocity += SteeringOutputAlign.AngularAcceleration * DeltaTime;
		// //damping suave
		// //currentAngularVelocity *= 0.95f;
		// float newOrientation = GetOrientation() + currentAngularVelocity * DeltaTime;
		// SetOrientation(newOrientation);

		//Path
		//CurrentVelocity += SteeringOutputPath.LinearAcceleration * DeltaTime;
		CurrentVelocity += blended * DeltaTime;
		
		// Clamp velocity
		if (CurrentVelocity.Size() > m_params.max_velocity)
		{
			CurrentVelocity = CurrentVelocity.GetSafeNormal() * m_params.max_velocity;
		}

		// Update position
		FVector newPos = GetActorLocation() + CurrentVelocity * DeltaTime;
		SetActorLocation(newPos);

		// --- Optional: Orientation update (simplified version) ---
		// if (!CurrentVelocity.IsNearlyZero())
		// {
		// 	FRotator newRot = CurrentVelocity.Rotation(); // Convert to rotator
		// 	SetActorRotation(FRotator(0.f, newRot.Yaw, 0.f));
		// }
	}
	DrawDebug();
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::OnClickedLeft(const FVector& mousePosition)
{
	SetActorLocation(mousePosition);
}

void AAICharacter::OnClickedRight(const FVector& mousePosition)
{
	m_params.targetPosition = mousePosition;

	FVector dir = (mousePosition - GetActorLocation()).GetSafeNormal();
	float angle = FMath::RadiansToDegrees(atan2(dir.Z, dir.X));
	m_params.targetRotation = angle;
}

void AAICharacter::DrawDebug()
{
	// TArray<FVector> Points =
	// {
	// 	FVector(0.f, 0.f, 0.f),
	// 	FVector(100.f, 0.f, 0.f),
	// 	FVector(100.f, 0.f, 100.f),
	// 	FVector(100.f, 0.f, 100.f),
	// 	FVector(0.f, 0.f, 100.f)
	// };

	//SetPath(this, TEXT("follow_path"), TEXT("path"), m_params.PathPoints, 20.0f, PathMaterial);

	// Pintar puntos del path
	for (int32 i = 0; i < m_params.PathPoints.Num(); ++i)
	{
		//DrawDebugLine(GetWorld(), PathPoints[i], PathPoints[i+1], FColor::Yellow, false, -1, 0, 3.f);
		DrawDebugSphere(GetWorld(), m_params.PathPoints[i], 10.f, 8, FColor::Blue, false, -1, 0, 2.f);
	}
	
	SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, 20.0f);

	FString ActorName;
	for (int32 i = 0; i < m_params.Obstacles.Num(); ++i)
	{
		if (i == 0)
		{
			ActorName = TEXT("Obstacle_0");
		}
		if (i == 1)
		{
			ActorName = TEXT("Obstacle_1");
		}
		SetCircle(this,ActorName,m_params.Obstacles[i].position,  m_params.Obstacles[i].radius, FColor::White);
	}

	// FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
	// SetArrow(this, TEXT("targetRotation"), dir, 80.0f);

	// TArray<TArray<FVector>> Polygons = {
	// 	{ FVector(0.f, 0.f, 0.f), FVector(100.f, 0.f, 0.f), FVector(100.f, 0.f, 100.0f), FVector(0.f, 0.f, 100.0f) },
	// 	{ FVector(100.f, 0.f, 0.f), FVector(200.f, 0.f, 0.f), FVector(200.f, 0.f, 100.0f) }
	// };
	//SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), Polygons, NavmeshMaterial);

	if (m_steeringBehaviour)
	{
		m_steeringBehaviour->DrawDebug();
	}
	if (m_steeringBehaviour2)
	{
		m_steeringBehaviour2->DrawDebug();
	}
}