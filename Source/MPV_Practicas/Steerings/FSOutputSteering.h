#pragma once

struct FSOutputSteering
{
	FVector LinearAcceleration;
	float AngularAcceleration;

	FSOutputSteering() : LinearAcceleration(FVector::ZeroVector), AngularAcceleration(0.f) {}
};
