#include "ObjectManager.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	for (std::map<std::string, SkinnedModel*>::iterator itr = skinnedModels.begin(); itr != skinnedModels.end(); ++itr)
	{
		delete itr->second;
	}
	skinnedModels.clear();

	for (std::map<std::string, BasicModel*>::iterator itr = staticModels.begin(); itr != staticModels.end(); ++itr)
	{
		delete itr->second;
	}
	staticModels.clear();
}

bool ObjectManager::Init(ID3D11Device *device, ID3D11DeviceContext *context)
{
	this->device = device;
	this->context = context;

	texMgr.Init(device);

	// Here Load All Objects you Use
	std::vector<std::string> anims;
	anims.push_back("res/Models/Victoria/idle.fbx");
	/*anims.push_back("Models/Daisy/crouching_turn_90_right.fbx");
	anims.push_back("Models/Daisy/death_crouching_headshot_front.fbx");
	anims.push_back("Models/Daisy/death_from_back_headshot.fbx");
	anims.push_back("Models/Daisy/death_from_front_headshot.fbx");
	anims.push_back("Models/Daisy/death_from_right.fbx");
	anims.push_back("Models/Daisy/death_from_the_back.fbx");
	anims.push_back("Models/Daisy/death_from_the_front.fbx");
	anims.push_back("Models/Daisy/idle_aiming.fbx");
	anims.push_back("Models/Daisy/idle_crouching.fbx");
	anims.push_back("Models/Daisy/idle_crouching_aiming.fbx");
	anims.push_back("Models/Daisy/jump_down.fbx");
	anims.push_back("Models/Daisy/jump_loop.fbx");
	anims.push_back("Models/Daisy/jump_up.fbx");
	anims.push_back("Models/Daisy/run_backward.fbx");
	anims.push_back("Models/Daisy/run_backward_left.fbx");
	anims.push_back("Models/Daisy/run_backward_right.fbx");
	anims.push_back("Models/Daisy/run_forward.fbx");
	anims.push_back("Models/Daisy/run_forward_left.fbx");
	anims.push_back("Models/Daisy/run_forward_right.fbx");
	anims.push_back("Models/Daisy/run_left.fbx");
	anims.push_back("Models/Daisy/run_right.fbx");
	anims.push_back("Models/Daisy/sprint_backward.fbx");
	anims.push_back("Models/Daisy/sprint_backward_left.fbx");
	anims.push_back("Models/Daisy/sprint_backward_right.fbx");
	anims.push_back("Models/Daisy/sprint_forward.fbx");
	anims.push_back("Models/Daisy/sprint_forward_left.fbx");
	anims.push_back("Models/Daisy/sprint_forward_right.fbx");
	anims.push_back("Models/Daisy/sprint_left.fbx");
	anims.push_back("Models/Daisy/sprint_right.fbx");
	anims.push_back("Models/Daisy/turn_90_left.fbx");
	anims.push_back("Models/Daisy/turn_90_right.fbx");
	anims.push_back("Models/Daisy/walk_backward.fbx");
	anims.push_back("Models/Daisy/walk_backward_left.fbx");
	anims.push_back("Models/Daisy/walk_backward_right.fbx");
	anims.push_back("Models/Daisy/walk_crouching_backward.fbx");
	anims.push_back("Models/Daisy/walk_crouching_backward_left.fbx");
	anims.push_back("Models/Daisy/walk_crouching_backward_right.fbx");
	anims.push_back("Models/Daisy/walk_crouching_forward.fbx");
	anims.push_back("Models/Daisy/walk_crouching_forward_left.fbx");
	anims.push_back("Models/Daisy/walk_crouching_forward_right.fbx");
	anims.push_back("Models/Daisy/walk_crouching_left.fbx");
	anims.push_back("Models/Daisy/walk_crouching_right.fbx");
	anims.push_back("Models/Daisy/walk_forward.fbx");
	anims.push_back("Models/Daisy/walk_forward_left.fbx");
	anims.push_back("Models/Daisy/walk_forward_right.fbx");
	anims.push_back("Models/Daisy/walk_left.fbx");
	anims.push_back("Models/Daisy/walk_right.fbx");*/

	std::string fileName = "res/Models/Victoria/idle.fbx";
	std::string objName = "Daisy";
	
	AddObjectStatic(&fileName, &objName);
	
	/*anims.clear();
	fileName.clear();
	objName.clear();

	fileName = "Models/Enemy/idle.fbx";
	objName = "Enemy";
	
	anims.push_back("Models/Enemy/crouching_turn_90_left.fbx");
	anims.push_back("Models/Enemy/crouching_turn_90_right.fbx");
	anims.push_back("Models/Enemy/death_crouching_headshot_front.fbx");
	anims.push_back("Models/Enemy/death_from_back_headshot.fbx");
	anims.push_back("Models/Enemy/death_from_front_headshot.fbx");
	anims.push_back("Models/Enemy/death_from_right.fbx");
	anims.push_back("Models/Enemy/death_from_the_back.fbx");
	anims.push_back("Models/Enemy/death_from_the_front.fbx");
	anims.push_back("Models/Enemy/idle_aiming.fbx");
	anims.push_back("Models/Enemy/idle_crouching.fbx");
	anims.push_back("Models/Enemy/idle_crouching_aiming.fbx");
	anims.push_back("Models/Enemy/jump_down.fbx");
	anims.push_back("Models/Enemy/jump_loop.fbx");
	anims.push_back("Models/Enemy/jump_up.fbx");
	anims.push_back("Models/Enemy/run_backward.fbx");
	anims.push_back("Models/Enemy/run_backward_left.fbx");
	anims.push_back("Models/Enemy/run_backward_right.fbx");
	anims.push_back("Models/Enemy/run_forward.fbx");
	anims.push_back("Models/Enemy/run_forward_left.fbx");
	anims.push_back("Models/Enemy/run_forward_right.fbx");
	anims.push_back("Models/Enemy/run_left.fbx");
	anims.push_back("Models/Enemy/run_right.fbx");
	anims.push_back("Models/Enemy/sprint_backward.fbx");
	anims.push_back("Models/Enemy/sprint_backward_left.fbx");
	anims.push_back("Models/Enemy/sprint_backward_right.fbx");
	anims.push_back("Models/Enemy/sprint_forward.fbx");
	anims.push_back("Models/Enemy/sprint_forward_left.fbx");
	anims.push_back("Models/Enemy/sprint_forward_right.fbx");
	anims.push_back("Models/Enemy/sprint_left.fbx");
	anims.push_back("Models/Enemy/sprint_right.fbx");
	anims.push_back("Models/Enemy/turn_90_left.fbx");
	anims.push_back("Models/Enemy/turn_90_right.fbx");
	anims.push_back("Models/Enemy/walk_backward.fbx");
	anims.push_back("Models/Enemy/walk_backward_left.fbx");
	anims.push_back("Models/Enemy/walk_backward_right.fbx");
	anims.push_back("Models/Enemy/walk_crouching_backward.fbx");
	anims.push_back("Models/Enemy/walk_crouching_backward_left.fbx");
	anims.push_back("Models/Enemy/walk_crouching_backward_right.fbx");
	anims.push_back("Models/Enemy/walk_crouching_forward.fbx");
	anims.push_back("Models/Enemy/walk_crouching_forward_left.fbx");
	anims.push_back("Models/Enemy/walk_crouching_forward_right.fbx");
	anims.push_back("Models/Enemy/walk_crouching_left.fbx");
	anims.push_back("Models/Enemy/walk_crouching_right.fbx");
	anims.push_back("Models/Enemy/walk_forward.fbx");
	anims.push_back("Models/Enemy/walk_forward_left.fbx");
	anims.push_back("Models/Enemy/walk_forward_right.fbx");
	anims.push_back("Models/Enemy/walk_left.fbx");
	anims.push_back("Models/Enemy/walk_right.fbx");

	AddObjectSkinned(&fileName, &objName, anims);

	anims.clear();
	fileName.clear();
	objName.clear();

	fileName = "Models/Monster/mutant_breathing_idle.fbx";
	objName = "Enemy";

	anims.push_back("Models/Monster/jump_attack.fbx");
	anims.push_back("Models/Monster/mutant_flexing_muscles.fbx");
	anims.push_back("Models/Monster/mutant_jump_attack.fbx");
	anims.push_back("Models/Monster/mutant_roaring.fbx");

	AddObjectSkinned(&fileName, &objName, anims);*/

	anims.clear();
	fileName.clear();
	objName.clear();

	return true;
}

BasicModel* ObjectManager::GetStatic(std::string objName)
{
	return staticModels[objName];
}

SkinnedModel* ObjectManager::GetSkinned(std::string objName)
{
	return skinnedModels[objName];
}

void ObjectManager::AddObjectSkinned(std::string *filePath, const std::string *objName, std::vector<std::string> animsPath)
{
	if (device == NULL)
	{
		MessageBox(0, L"ObjectManager wasn't initialized", 0, 0);
		exit(0);
	}

	skinnedModels.insert(std::pair<std::string, SkinnedModel*>(*objName, new SkinnedModel(device, texMgr, *filePath, L"Textures/")));

	for (unsigned int i = 0; i < animsPath.size(); ++i)
	{
		skinnedModels[*objName]->AddAnim(animsPath[i]);
	}
}

void ObjectManager::AddObjectStatic(std::string *filePath, const std::string *objName)
{
	if (device == NULL)
	{
		MessageBox(0, L"ObjectManager wasn't initialized", 0, 0);
		exit(0);
	}

	BasicModel *tmp = new BasicModel(device, texMgr, *filePath, L"Textures/", false);

	staticModels.insert(std::pair<std::string, BasicModel*>(*objName, tmp));
}

void ObjectManager::UnloadSkinned(const std::string *objName)
{
	skinnedModels.erase(*objName);
}

void ObjectManager::UnloadStatic(const std::string *objName)
{
	staticModels.erase(*objName);
}
