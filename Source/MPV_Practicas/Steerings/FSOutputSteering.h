#pragma once

struct FSOutputSteering
{
	FVector LinearAcceleration;
	float AngularAcceleration;
	bool stop = false;
	
	FSOutputSteering() : LinearAcceleration(FVector::ZeroVector), AngularAcceleration(0.f) {}
};
