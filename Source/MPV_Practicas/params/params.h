#ifndef __PARAMS_H__
#define __PARAMS_H__

#include <CoreMinimal.h>

struct Params
{
    FVector targetPosition;
    float max_velocity;
    float max_acceleration;

    float targetRotation;
    float max_angular_velocity;
    float max_angular_acceleration;

    float dest_radius;
    float arrive_radius;
    float align_radius;
    float angular_arrive_radius;

    float look_ahead;
    float time_ahead;
    TArray<FVector> PathPoints;
};

bool ReadParams(const char* filename, Params& params);
bool ReadPath(const char* filename, Params& params);

#endif