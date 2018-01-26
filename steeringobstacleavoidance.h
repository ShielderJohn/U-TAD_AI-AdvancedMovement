#ifndef __STEERINGOBSTACLEAVOIDANCE_H__
#define __STEERINGOBSTACLEAVOIDANCE_H__

#include "steering.h"

class SteeringObstacleAvoidance : public Steering
{
public:
	SteeringObstacleAvoidance();
	~SteeringObstacleAvoidance();

	virtual USVec2D GetSteering(Character* character, const USVec2D& target);
	virtual float GetAngularSteering(Character* character, float targetRotation) { return 0.0f; }
	virtual void DrawDebug(Character* character);

private:
	USVec2D mVelocity;
	USVec2D mAcceleration;
};

#endif