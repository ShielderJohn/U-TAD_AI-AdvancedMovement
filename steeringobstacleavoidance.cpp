#include "stdafx.h"
#include "steeringobstacleavoidance.h"
#include "character.h"
#include "params.h"

SteeringObstacleAvoidance::SteeringObstacleAvoidance()
{
	mVelocity = USVec2D(0.0f, 0.0f);
	mAcceleration = USVec2D(0.0f, 0.0f);
}

SteeringObstacleAvoidance::~SteeringObstacleAvoidance()
{

}


USVec2D SteeringObstacleAvoidance::GetSteering(Character* character, const USVec2D& target)
{
	mAcceleration.mX = 0.0f;
	mAcceleration.mY = 0.0f;

	Params params = character->GetParams();
	Obstacles obstacles = character->GetObstacles();

	int numberOfObstacles = obstacles.mObstacles.size();

	USVec2D np = USVec2D(0.0f, 0.0f);
	USVec2D lp = USVec2D(0.0f, 0.0f);

	USVec2D ro = USVec2D(0.0f, 0.0f);
	float projection = 0.0f;
	USVec2D near = USVec2D(0.0f, 0.0f);
	USVec2D difference = USVec2D(0.0f, 0.0f);
	float distance = 0.0f;

	USVec2D rorodifference = USVec2D(0.0f, 0.0f);

	// np = normalize(v)
	np = character->GetLinearVelocity();
	np.NormSafe();

	// lp = np * look_ahead
	lp = np * params.look_ahead;

	for (int i = 0; i < numberOfObstacles; ++i)
	{
		// ro = obstacleLocation - characterLocation
		ro = obstacles.mObstacles[i].mLocation - character->GetLoc();

		// projection = ro · np
		projection = ro.Dot(np);

		if (projection > lp.Length())
		{
			// near = characterLocation + lp
			near = character->GetLoc() + lp;
		}
		else if (projection >= 0.0f)
		{
			// near = characterLocation + np * projection
			near = character->GetLoc() + np * projection;
		}

		// difference = near - obstacleLocation
		difference = near - obstacles.mObstacles[i].mLocation;

		// distance = len(difference)
		distance = difference.Length();

		if (distance <= obstacles.mObstacles[i].mRadius)
		{
			rorodifference = USVec2D(ro.mX * ro.mX * difference.mX, ro.mY * ro.mY * difference.mY);

			mAcceleration += rorodifference;
		}

		if (mAcceleration.Length() != 0.0f)
		{
			USVec2D normalizedAcceleration = mAcceleration;
			normalizedAcceleration.NormSafe();

			return normalizedAcceleration * character->GetParams().max_acceleration;
		}
	}
	
	return USVec2D(0.0f, 0.0f);
}

void SteeringObstacleAvoidance::DrawDebug(Character* character)
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	Obstacles obstacles = character->GetObstacles();
	int numberOfObstacles = obstacles.mObstacles.size();
	float distanceToObstacle = 0.0f;

	for (int i = 0; i < numberOfObstacles; ++i)
	{
		distanceToObstacle = (obstacles.mObstacles[i].mLocation - character->GetLoc()).Length();

		if (distanceToObstacle <= obstacles.mObstacles[i].mRadius + character->GetParams().char_radius)
		{
			gfxDevice.SetPenColor(0.25f, 0.85f, 0.61f, 1.0f);
		}
		else
		{
			gfxDevice.SetPenColor(1.0f, 0.0f, 0.0f, 1.0f);
		}

		MOAIDraw::DrawEllipseOutline(obstacles.mObstacles[i].mLocation.mX, obstacles.mObstacles[i].mLocation.mY, obstacles.mObstacles[i].mRadius, obstacles.mObstacles[i].mRadius, 500);
	}
}