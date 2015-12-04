#pragma once

#include "Helper.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "Level.h"
#include "Level0.h"

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	bool Init(ID3D11Device *device, ID3D11DeviceContext *context, ObjectManager *om);
	void Update(float dt);
	void Draw(ID3D11Device *device, ID3D11DeviceContext *context);
	void OnResize(int width, int height, float aspect);

	void SetCamera(Camera *cam);

	Level* GetLevel();

private:
	Level::LevelStatus currLevelStatus;
	ObjectManager *aObjManager;

	Level *level;

	ID3D11Device *device;
	ID3D11DeviceContext *context;
	Camera *cam;
};

