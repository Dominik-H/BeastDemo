#pragma once

#include "Helper.h"
#include "Meshes\BasicModel.h"
#include "Meshes\SkinnedModel.h"
#include "TextureMgr.h"
#include <map>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	bool Init(ID3D11Device *device, ID3D11DeviceContext *context);

	BasicModel* GetStatic(std::string objName);
	SkinnedModel* GetSkinned(std::string objName);

	void AddObjectSkinned(std::string *filePath, const std::string *objName, std::vector<std::string> animsPath);
	void AddObjectStatic(std::string *filePath, const std::string *objName);

	void UnloadSkinned(const std::string *objName);
	void UnloadStatic(const std::string *objName);
private:
	std::map<std::string, BasicModel*> staticModels;
	std::map<std::string, SkinnedModel*> skinnedModels;

	ID3D11Device *device;
	ID3D11DeviceContext *context;
	TextureMgr texMgr;
};

