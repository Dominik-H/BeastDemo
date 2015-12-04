#include "LevelManager.h"


LevelManager::LevelManager()
{
}


LevelManager::~LevelManager()
{
	DEL(level);
}

bool LevelManager::Init(ID3D11Device *device, ID3D11DeviceContext *context, ObjectManager *om)
{
	aObjManager = om;
	this->device = device;
	this->context = context;

	level = new Level0();
	level->LoadLevel(device, context);
	currLevelStatus = Level::LevelStatus::LEVEL_RUNNING;

	//CollisionManager::Levels.push_back(level);

	return true;
}

void LevelManager::Update(float dt)
{
	if (currLevelStatus == Level::LevelStatus::LEVEL_RUNNING)
	{
		level->Update(dt);
	}
}

void LevelManager::Draw(ID3D11Device *device, ID3D11DeviceContext *context)
{
	level->Draw(device, context);
}

void LevelManager::OnResize(int width, int height, float aspect)
{

}

void LevelManager::SetCamera(Camera *cam)
{
	this->cam = cam;
	level->SetCam(cam);
}

Level* LevelManager::GetLevel()
{
	return level;
}
