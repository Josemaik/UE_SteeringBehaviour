#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "../Structs/Obstacle.h"
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
    float char_radius;
    TArray<FVector> PathPoints;
    TArray<Obstacle> Obstacles;
};

bool ReadParams(const char* filename, Params& params);
bool ReadPath(const char* filename, Params& params);
bool ReadObstacles(const char* filename, Params& params);

#endif