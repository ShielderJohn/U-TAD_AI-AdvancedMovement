#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include "steeringseek.h"
#include "steeringarrive.h"
#include "steeringalign.h"
#include "steeringaligntomovement.h"
#include "steeringpathfollowing.h"
#include "steeringobstacleavoidance.h"

Character::Character() : mLinearVelocity(0.0f, 0.0f), mAngularVelocity(0.0f)
{
	RTTI_BEGIN
		RTTI_EXTEND(MOAIEntity2D)
	RTTI_END

	//mSteerings.push_back(new SteeringSeek());
	//mSteerings.push_back(new SteeringArrive());
	//mSteerings.push_back(new SteeringAlign());

	mSteerings.push_back(new SteeringAlignToMovement());
	mWeights.push_back(1.0f);

	mSteerings.push_back(new SteeringObstacleAvoidance());
	mWeights.push_back(1.0f);

	mSteerings.push_back(new SteeringPathFollowing());
	mWeights.push_back(0.25f);
}

Character::~Character()
{
	mSteerings.clear();
}

void Character::OnStart()
{
    ReadParams("params.xml", mParams);
	ReadPath("path.xml", mPath);
	ReadObstacles("obstacles.xml", mObstacles);
}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{
	USVec2D linearAcceleration = USVec2D(0.0f, 0.0f);
	float angularAcceleraton = 0.0f;

	std::vector<USVec2D> linearAccelerations;
	std::vector<float> angularAccelerations;

	float linearAccelerationTotalWeight = 0.0f;
	float angularAccelerationTotalWeight = 0.0f;

	int numberOfSteerings = mSteerings.size();

	for (int i = 0; i < numberOfSteerings; ++i)
	{
		linearAccelerations.push_back(mSteerings[i]->GetSteering(this, mParams.targetPosition));

		if (linearAccelerations[i].LengthSquared() != 0)
		{
			linearAccelerationTotalWeight += mWeights[i];
		}

		angularAccelerations.push_back(mSteerings[i]->GetAngularSteering(this, mParams.targetRotation));

		if (angularAccelerations[i] != 0)
		{
			angularAccelerationTotalWeight += mWeights[i];
		}
	}

	for (int i = 0; i < numberOfSteerings; ++i)
	{
		linearAcceleration += linearAccelerations[i] * mWeights[i] / linearAccelerationTotalWeight;
		angularAcceleraton += angularAccelerations[i] * mWeights[i] / angularAccelerationTotalWeight;
	}

	mLinearVelocity += linearAcceleration * step;
	mAngularVelocity += angularAcceleraton * step;

	// Clamp to max velocity
	if (mLinearVelocity.Length() > mParams.max_velocity)
	{
		mLinearVelocity.NormSafe();
		mLinearVelocity *= mParams.max_velocity;
	}

	SetLoc(GetLoc() + mLinearVelocity * step);

	// Clamp to max angular velocity
	if (abs(mAngularVelocity) > mParams.max_angular_acceleration)
	{
		if (mAngularVelocity > 0.0f)
		{
			mAngularVelocity = mParams.max_angular_velocity;
		}
		else if (mAngularVelocity < 0.0f)
		{
			mAngularVelocity = -mParams.max_angular_velocity;
		}
	}

	SetRot(GetRot() + mAngularVelocity * step);
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();
	//gfxDevice.SetPenColor(0.0f, 0.0f, 1.0f, 0.5f);
	//MOAIDraw::DrawPoint(0.0f, 0.0f);

	// Orientation
	gfxDevice.SetPenColor(1.0f, 1.0f, 1.0f, 1.0f);
	MOAIDraw::DrawLine(static_cast<USVec2D>(GetLoc()), static_cast<USVec2D>(GetLoc()) + mLinearVelocity);

	int numberOfSteerings = mSteerings.size();

	for (int i = 0; i < numberOfSteerings; ++i)
	{
		mSteerings[i]->DrawDebug(this);
	}
}



// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);

	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")

	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
