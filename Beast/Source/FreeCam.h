#pragma once

#include <dinput.h>
#include "Helper.h"
#include "Camera.h"
#include "LevelManager.h"
#include "ObjectManager.h"

class FreeCam
{
public:
	FreeCam();
	~FreeCam();

	bool Init(ID3D11Device *device, ID3D11DeviceContext *context, IDXGISwapChain *sc, HINSTANCE hInstance, HWND hwnd, LevelManager* lm, ObjectManager *om);
	void Update(float dt);
	void OnResize(int width, int height, float aspect);
	void DetectInput(float dt, HWND hwnd);

	Camera *GetCam();
	void SetCam(Camera *cam);

private:
	bool InitDirectInput(HINSTANCE hInst, HWND hwnd);

	Camera *aCam;

	IDirectInputDevice8* aDIKeyboard;
	IDirectInputDevice8* aDIMouse;

	DIMOUSESTATE aMouseLastState;
	LPDIRECTINPUT8 aDirectInput;

	LevelManager* aLevelManager;
	ObjectManager *aObjManager;

	int camPos;
};

