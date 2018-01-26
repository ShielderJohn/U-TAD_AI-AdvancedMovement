#ifndef __STEERINGARRIVE_H__
#define __STEERINGARRIVE_H__

#include "steering.h"

class SteeringArrive : public Steering
{
public:
	SteeringArrive();
	~SteeringArrive();

	virtual USVec2D GetSteering(Character* character, const USVec2D& target);
	virtual float GetAngularSteering(Character* character, float targetRotation) { return 0.0f; }
	virtual void DrawDebug(Character* character);

private:
	USVec2D mVelocity;
	USVec2D mAcceleration;

	USVec2D mCharacterPosition;

	float mVelocityLength;
};

#endif