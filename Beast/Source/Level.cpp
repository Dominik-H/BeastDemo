#include "Level.h"


Level::Level()
	:sky(0), cam(0)
{
}


Level::~Level()
{
}

Grid* Level::GetGrid()
{
	return &grid;
}

Skybox* Level::GetSky()
{
	return sky;
}

void Level::AddObject(BasicModelInstance *obj)
{
	objects.push_back(obj);
}

DirectionalLight* Level::GetDirLights()
{
	return dirLights;
}

void Level::SetCam(Camera *cam)
{
	this->cam = cam;
}
