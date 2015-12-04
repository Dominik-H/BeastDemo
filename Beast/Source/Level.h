#pragma once

#include "Objects\Grid.h"
#include "Objects\Skybox.h"
#include "Meshes\BasicModel.h"
#include "Structs.h"
#include "Collider.h"

class Level
{
public:
	enum LevelStatus{LEVEL_INIT = 0, LEVEL_RUNNING, LEVEL_END};

	Level();
	virtual ~Level();

	virtual bool LoadLevel(ID3D11Device *device, ID3D11DeviceContext *context) = 0; 
	virtual void Update(float dt) = 0;
	virtual void Draw(ID3D11Device *device, ID3D11DeviceContext *context) = 0;

	void AddObject(BasicModelInstance *obj);
	void SetCam(Camera *cam);

	Grid* GetGrid();
	Skybox* GetSky();
	DirectionalLight* GetDirLights();

	Collider Collider;

protected:
	std::vector<BasicModelInstance *> objects;
	Grid grid;
	Skybox *sky;

	DirectionalLight dirLights[3];
	Camera *cam;
};

