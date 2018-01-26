#include "stdafx.h"
#include <tinyxml.h>
#include "obstacle.h"

bool ReadObstacles(const char* filename, Obstacles& obstacles)
{
	TiXmlDocument doc(filename);
	if (!doc.LoadFile())
	{
		fprintf(stderr, "Couldn't read path from %s", filename);
		return false;
	}

	TiXmlHandle hDoc(&doc);

	TiXmlElement* pElem;
	pElem = hDoc.FirstChildElement().Element();
	if (!pElem)
	{
		fprintf(stderr, "Invalid format for %s", filename);
		return false;
	}

	TiXmlHandle hRoot(pElem);
	TiXmlHandle hObstacles = hRoot.FirstChildElement("obstacles");

	TiXmlElement* obstacleElem = hObstacles.FirstChild().Element();

	for (obstacleElem; obstacleElem; obstacleElem = obstacleElem->NextSiblingElement())
	{
		const char* pointName = obstacleElem->Value();

		if (!strcmp(pointName, "obstacle"))
		{
			float x = 0.0f;
			float y = 0.0f;
			float radius = 0.0f;

			obstacleElem->Attribute("x", &x);
			obstacleElem->Attribute("y", &y);
			obstacleElem->Attribute("radius", &radius);

			obstacles.mObstacles.push_back({ { x, y }, radius });
		}
	}

	return true;
}