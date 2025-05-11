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
	m_steeringBehaviour = new AlignSteering(this);
	m_steeringBehaviour2 = new ArriveSteering(this);
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
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	current_angle = GetActorAngle();

	DrawDebug();

	//Aply steering
	if (m_steeringBehaviour && m_steeringBehaviour2)
	{
		FSOutputSteering SteeringOutput = m_steeringBehaviour->GetSteering(DeltaTime);
		FSOutputSteering SteeringOutput2 = m_steeringBehaviour2->GetSteering(DeltaTime);
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
		CurrentVelocity += SteeringOutput2.LinearAcceleration * DeltaTime;
		 //Clamp
		 if (CurrentVelocity.Length() > m_params.max_velocity)
		 {
		 	CurrentVelocity = CurrentVelocity.GetClampedToMaxSize(m_params.max_velocity);
		 }
		//Update position
		FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
		SetActorLocation(NewLocation);

		//Align
		currentAngularVelocity += SteeringOutput.AngularAcceleration * DeltaTime;
		float newOrientation = GetOrientation() + currentAngularVelocity * DeltaTime;
		SetOrientation(newOrientation);
	}
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
	m_params.targetRotation = angle + 10.f;
}

void AAICharacter::DrawDebug()
{
	TArray<FVector> Points =
	{
		FVector(0.f, 0.f, 0.f),
		FVector(100.f, 0.f, 0.f),
		FVector(100.f, 0.f, 100.f),
		FVector(100.f, 0.f, 100.f),
		FVector(0.f, 0.f, 100.f)
	};

	SetPath(this, TEXT("follow_path"), TEXT("path"), Points, 5.0f, PathMaterial);

	SetCircle(this, TEXT("targetPosition"), m_params.targetPosition, 20.0f);
	// FVector dir(cos(FMath::DegreesToRadians(m_params.targetRotation)), 0.0f, sin(FMath::DegreesToRadians(m_params.targetRotation)));
	// SetArrow(this, TEXT("targetRotation"), dir, 80.0f);

	TArray<TArray<FVector>> Polygons = {
		{ FVector(0.f, 0.f, 0.f), FVector(100.f, 0.f, 0.f), FVector(100.f, 0.f, 100.0f), FVector(0.f, 0.f, 100.0f) },
		{ FVector(100.f, 0.f, 0.f), FVector(200.f, 0.f, 0.f), FVector(200.f, 0.f, 100.0f) }
	};
	SetPolygons(this, TEXT("navmesh"), TEXT("mesh"), Polygons, NavmeshMaterial);

	if (m_steeringBehaviour)
	{
		m_steeringBehaviour->DrawDebug();
	}
	if (m_steeringBehaviour2)
	{
		m_steeringBehaviour2->DrawDebug();
	}
}