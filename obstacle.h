#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

#include <vector>

struct Obstacle
{
	USVec2D mLocation;
	float mRadius;
};

struct Obstacles
{
	std::vector<Obstacle> mObstacles;
};

bool ReadObstacles(const char* filename, Obstacles& obstacles);

#endif