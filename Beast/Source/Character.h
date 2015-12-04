#pragma once

#include <dinput.h>
#include "Helper.h"
#include "Effects.h"
#include "Camera.h"
#include "RenderStates.h"
#include "Meshes/SkinnedModel.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"

class Character
{
public:
	Character();
	~Character();

	bool Init(ID3D11Device *device, ID3D11DeviceContext *context, IDXGISwapChain *sc, HINSTANCE hInstance, HWND hwnd, LevelManager* lm, ObjectManager *om);
	void Update(float dt);
	void Draw(ID3D11Device *device, ID3D11DeviceContext *context);
	void OnResize(int width, int height, float aspect);
	void DetectInput(float dt, HWND hwnd);

	Camera *GetCam();
	void SetCam(Camera *cam);

private:
	bool InitDirectInput(HINSTANCE hInst, HWND hwnd);
	
	Camera *aCam;

	SkinnedModelInstance aCharInst;
	BasicModelInstance aModInst;
	UINT myColiderPos;
	//BasicModelInstance aCharInst;
	XMVECTOR aModelPos;

	IDirectInputDevice8* aDIKeyboard;
	IDirectInputDevice8* aDIMouse;

	DIMOUSESTATE aMouseLastState;
	LPDIRECTINPUT8 aDirectInput;

	LevelManager* aLevelManager;
	ObjectManager *aObjManager;

	int charPos, camPos;
};

