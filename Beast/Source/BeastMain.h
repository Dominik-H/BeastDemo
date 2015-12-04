#if 0
/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This class connects everything.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef BEAST_MAIN_H
#define BEAST_MAIN_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>
#include "Objects/Box.h"
#include "Objects/Grid.h"
#include "Objects/Skybox.h"
#include "Objects/Terrain.h"
#include "Meshes/Model.h"
#include "Effects.h"
#include "Camera.h"
#include "RenderStates.h"
#include "Meshes/SkinnedModel.h"
#include "TextureMgr.h"
#include "Meshes\BasicModel.h"

class Beast
{
	friend class GameCore;

public: 
	Beast( void );
	~Beast( void );

	void Update( float dt );
	void Draw( ID3D11Device* device, ID3D11DeviceContext* context );

	//--------------------------------------------------
	// Input Handling
	//--------------------------------------------------
	bool InitDirectInput( HINSTANCE hInst, HWND hwnd );
	void DetectInput( float time, HWND hwnd );

	bool Init( ID3D11Device* device,	ID3D11DeviceContext* context, IDXGISwapChain* sc, HINSTANCE hInst, HWND hwnd );
	void OnResize( int width, int height, float aspect );

private:
	//--------------------------------------------------
	// Objects
	//--------------------------------------------------
	Camera cam;

	DirectionalLight dirLights[3];
	
	Box aBox;
	//Grid ground;
	Model16 map;
	Terrain terrain;
	Skybox* sky;

	SkinnedModel* charModel;
	SkinnedModelInstance modelInst1;
	XMVECTOR modelPos;
	void DrawModel( ID3D11Device* device, ID3D11DeviceContext* context );

	//--------------------------------------------------
	// Textures
	//--------------------------------------------------
	Texture boxText;
	TextureMgr texMgr;
	Texture groundText;

	//--------------------------------------------------
	// Input
	//--------------------------------------------------
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;

	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;
};

#endif // BEAST_MAIN_H

#endif