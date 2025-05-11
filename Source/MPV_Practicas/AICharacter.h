// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "params/params.h"
#include "GameFramework/PlayerController.h"
#include "util.h"


#include "AICharacter.generated.h"

class SteeringBehaviour;

UCLASS()
class MPV_PRACTICAS_API AAICharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAICharacter();

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
	uint32 bDoMovement : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIChar)
		float current_angle;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PathMaterial;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* NavmeshMaterial;
	
	UPROPERTY(EditAnywhere,Category = AIChar)
	FVector CurrentVelocity;

	// UPROPERTY(EditAnywhere,Category = AIChar)
	// float CurrentOrientation;

	UPROPERTY(EditAnywhere,Category = AIChar)
	float currentAngularVelocity = 1.f; 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	Params m_params;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "AIFunctions")
	void OnClickedLeft(const FVector& mousePosition);
	UFUNCTION(BlueprintCallable, Category = "AIFunctions")
	void OnClickedRight(const FVector& mousePosition);
	
	const Params& GetParams() const { return m_params; }

	FVector GetCurrentVelocity() const { return CurrentVelocity; }
	float GetCurretAngularVelocity() const { return currentAngularVelocity;}
	float GetOrientation() const
	{
		// float YawDegrees = GetActorRotation().Yaw;
		// float YawRadians = FMath::DegreesToRadians(YawDegrees);
		// return YawRadians;
		float PitchDegrees = GetActorRotation().Pitch;
		return FMath::DegreesToRadians(PitchDegrees);
	}
	void SetOrientation(float newOrientation)
	{
		// float YawDegrees = FMath::RadiansToDegrees(newOrientation);
		// FRotator NewRotation = FRotator(0.f, YawDegrees, 0.0f);
		// SetActorRotation(NewRotation);
		float PitchDegrees = FMath::RadiansToDegrees(newOrientation);
		FRotator CurrentRotation = FRotator(PitchDegrees,0.f,0.f);//GetActorRotation();
		CurrentRotation.Pitch = PitchDegrees;
		SetActorRotation(CurrentRotation);
	}
	float GetActorAngle() const
	{
		FQuat currQuat = GetActorQuat();
		FVector axis;
		float axisAngle;
		currQuat.ToAxisAndAngle(axis, axisAngle);
		axisAngle = FMath::RadiansToDegrees(axisAngle);
		if (axis.Y > 0.0f)
			axisAngle = -axisAngle;

		axisAngle = convertTo360(axisAngle);
		return axisAngle;
	}
	void SetActorAngle(float angle) { FRotator newRot(angle, 0.0f, 0.0f); SetActorRotation(newRot); }

	void DrawDebug();

	//steering behaviour
	SteeringBehaviour* m_steeringBehaviour;
	//stereing behaviour 2
	SteeringBehaviour* m_steeringBehaviour2;
};
