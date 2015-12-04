/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a header file to everything needed for initialization of the game.
*
* Copyright (c) Dominik Hornak. All rights reserved.
**************************************************************************************/
#ifndef INIT_H
#define INIT_H

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "Helper.h"
#include <string>
#include "Enums.h"
#include "GameTimer.h"
#include "Character.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "FreeCam.h"

class GameCore
{
public:
	GameCore( HINSTANCE hInstance );
	virtual ~GameCore( void );

	HINSTANCE GameInst() const;
	HWND MainWnd() const;
	float AspectRatio() const;

	int Run();

	virtual bool Init();
	virtual void OnResize();
	virtual void Update( float dt );
	virtual void Render();
	virtual LRESULT MsgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

private:
	LevelManager bLevels;
	Character bDaisy;
	FreeCam bFreeCam;
	ObjectManager bObjects;

	//--------------------------------------------------
	// Other -----> !!!!TODO - options etc...!!!!
	//--------------------------------------------------
	bool InitWindow();
	bool InitD3D();

	void CalculateFrameStats();
	GameTimer bTimer;

	HINSTANCE bGameInst;
	HWND			bMainWnd;
	bool			bGamePaused;
	bool			bMinimized;
	bool			bMaximized;
	bool			bResizing;
	UINT			b4xMSAAQuality;

	ID3D11Device*						bDevice;
	ID3D11DeviceContext*		bImmediateContext;
	IDXGISwapChain*					bSwapChain;
	ID3D11Texture2D*				bDepthStencilBuffer;
	ID3D11RenderTargetView*	bRenderTargetView;
	ID3D11DepthStencilView*	bDepthStencilView;
	D3D11_VIEWPORT					bScreenViewport;

	std::wstring		bMainWndCaption;
	D3D_DRIVER_TYPE	bd3dDriverType;
	int							bClientWidth;
	int							bClientHeight;
	MSAA						bMsaaLevel;
};

#endif // INIT_H