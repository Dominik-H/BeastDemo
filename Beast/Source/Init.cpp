/*************************************************************************************
* File: Init.h; Date of creation: 09.Feb.2015; Date of Last Modification: 12.Feb.2015
*
* This is a file that does everything needed for initialization of the game.
*
* Copyright (c) Dominik Hornak. All rights reserved.
**************************************************************************************/
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include "Init.h"
#include <WindowsX.h>
#include "resource.h"
#include "TextManager.h"

namespace
{
	// This is just used to forward Windows messages from a global window
	// procedure to our member function window procedure because we cannot
	// assign a member function to WNDCLASS::lpfnWndProc.
	GameCore* gTheGame = 0;
}

LRESULT CALLBACK
MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return gTheGame->MsgProc( hwnd, msg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd )
{
	// Enable run-time memory check for debug builds.
#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	GameCore theGame(hInstance);
	
	if( !theGame.Init() )
		return 0;
	
	return theGame.Run();
}

GameCore::GameCore( HINSTANCE hInstance )
:
	bGameInst( hInstance ),
	bMainWndCaption( L"Beast Project" ),
	bClientWidth( 800 ),
	bClientHeight( 600 ),
	bMainWnd( 0 ),
	bd3dDriverType( D3D_DRIVER_TYPE_HARDWARE ),
	bMsaaLevel( OFF ),
	bGamePaused( false ), 
	bMinimized( false ), 
	bMaximized( false ), 
	bResizing( false ), 
	b4xMSAAQuality( 0 ), 
	bDevice( 0 ), 
	bImmediateContext( 0 ), 
	bSwapChain( 0 ), 
	bDepthStencilBuffer( 0 ), 
	bRenderTargetView( 0 ), 
	bDepthStencilView( 0 )
{
	ZeroMemory( &bScreenViewport, sizeof( D3D11_VIEWPORT ) );

	gTheGame = this;
}

GameCore::~GameCore( void )
{
	Effects::Destroy();
	InputLayouts::Destroy();
	RenderStates::Destroy();
	//CollisionManager::Destroy();

	RELEASE( bRenderTargetView );
	RELEASE( bDepthStencilView );
	RELEASE( bSwapChain );
	RELEASE( bDepthStencilBuffer );

	if( bImmediateContext )
	{
		bImmediateContext->ClearState();
	}

	RELEASE( bImmediateContext );
	RELEASE( bDevice );
}

void GameCore::Update( float dt )
{
	//CollisionManager::CheckCollisions();
	bLevels.Update(dt);
	bDaisy.Update(dt);
	bFreeCam.Update(dt);
}

void GameCore::Render()
{
	//--------------------------------------------------
	// Clear the backbuffer
	//--------------------------------------------------
	float ClearColor[4] = { 0.00f, 0.0125f, 0.273f, 1.0f }; //red,green,blue,alpha
	bImmediateContext->ClearRenderTargetView( bRenderTargetView, ClearColor );
	bImmediateContext->ClearDepthStencilView( bDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	//--------------------------------------------------
	// Draw Everything
	//--------------------------------------------------
	//Effects::BasicFX->SetDirLights(/*TODO add dir lights [dirLights]*/);
	//Effects::BasicFX->SetEyePosW(/*TODO ADD Camera Pos [cam.GetPosition()]*/);

	bImmediateContext->RSSetState(RenderStates::NoCullRS);

	bLevels.Draw( bDevice, bImmediateContext);
	bDaisy.Draw(bDevice, bImmediateContext);

	//bObjects.Draw(bDevice, bImmediateContext);

	//bCollisions.Draw(bDevice, bImmediateContext); // buttons, interactions etc...
	/*
#if defined( DEBUG ) || defined( _DEBUG )
	CollisionManager::Draw(bDevice, bImmediateContext, bDaisy.GetCam()); // collision boxes etc...
#endif*/
	
	float blendFactor[4] = { 1.0f };
	bImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);

	TextManager::DrawAll(bImmediateContext);

	// restore default
	bImmediateContext->OMSetBlendState(0, blendFactor, 0xffffffff);

	//--------------------------------------------------
	// Swap Back & Front buffers
	//--------------------------------------------------
  bSwapChain->Present( 0, 0 );
}

HINSTANCE GameCore::GameInst() const
{
	return bGameInst;
}

HWND GameCore::MainWnd() const
{
	return bMainWnd;
}

float GameCore::AspectRatio() const
{
	return static_cast<float>( bClientWidth ) / bClientHeight;
}

int GameCore::Run()
{
	MSG msg = {0};
	bTimer.Reset();

	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} 
		else 
		{
			bTimer.Tick();

			if( !bGamePaused ) 
			{
				CalculateFrameStats();
				//bDaisy.DetectInput( bTimer.DeltaTime(), bMainWnd );
				bFreeCam.DetectInput(bTimer.DeltaTime(), bMainWnd);
				Update( bTimer.DeltaTime() );
				Render();
			} 
			else 
			{
				Sleep( 100 );
			}
		}
	}

	return ( int )msg.wParam;
}

bool GameCore::Init()
{
	if( !InitWindow() )
	{
		return false;
	}

	if( !InitD3D() )
	{
		return false;
	}

	Effects::Init(bDevice);
	InputLayouts::Init(bDevice);
	RenderStates::Init(bDevice);
	//CollisionManager::Init(bDevice);
	TextManager::Init(bDevice);

	if (!bObjects.Init(bDevice, bImmediateContext))
	{
		return false;
	}

	if (!bLevels.Init(bDevice, bImmediateContext, &bObjects))
	{
		return false;
	}

	if (!bFreeCam.Init(bDevice, bImmediateContext, bSwapChain, bGameInst, bMainWnd, &bLevels, &bObjects))
		return false;

	bDaisy.SetCam(bFreeCam.GetCam());

	if( !bDaisy.Init( bDevice, bImmediateContext, bSwapChain, bGameInst, bMainWnd, &bLevels, &bObjects) )
	{
		return false;
	}	

	/*for (UINT i = 0; i < CollisionManager::Animated.size(); ++i)
	{
		std::wstring txt = L"Animated colBox " + std::to_wstring((int)i) + L" " + std::to_wstring((int)CollisionManager::Animated[i]->Model->Collider.OBBs.size());
		TextManager::texts.push_back(txt);
	}
	for (UINT i = 0; i < CollisionManager::Static.size(); ++i)
	{
		std::wstring txt = L"Static colBox " + std::to_wstring((int)i) + L" " + std::to_wstring((int)CollisionManager::Static[i]->Model->Collider.OBBs.size());
		TextManager::texts.push_back(txt);
	}
	for (UINT i = 0; i < CollisionManager::Levels.size(); ++i)
	{
		std::wstring txt = L"Level colBox " + std::to_wstring((int)i) + L" " + std::to_wstring((int)CollisionManager::Levels[i]->Collider.OBBs.size());
		TextManager::texts.push_back(txt);
	}*/

	return true;
}

void GameCore::OnResize()
{
	assert( bImmediateContext );
	assert( bDevice );
	assert( bSwapChain );

	RELEASE( bRenderTargetView );
	RELEASE( bDepthStencilView );
	RELEASE( bDepthStencilBuffer );

	HR( bSwapChain->ResizeBuffers( 1, bClientWidth, bClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0 ) );
	ID3D11Texture2D *backBuffer;
	HR( bSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &backBuffer ) ) );
	HR( bDevice->CreateRenderTargetView( backBuffer, 0, &bRenderTargetView ) );
	RELEASE( backBuffer );

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width			= bClientWidth;
	depthStencilDesc.Height			= bClientHeight;
	depthStencilDesc.MipLevels	= 1;
	depthStencilDesc.ArraySize	= 1;
	depthStencilDesc.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;

	if( bMsaaLevel == MSAA8X ) 
	{
		depthStencilDesc.SampleDesc.Count		= 8;
		depthStencilDesc.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else if( bMsaaLevel == MSAA4X ) 
	{
		depthStencilDesc.SampleDesc.Count		= 4;
		depthStencilDesc.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else if( bMsaaLevel == MSAA2X ) 
	{
		depthStencilDesc.SampleDesc.Count		= 2;
		depthStencilDesc.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else 
	{
		depthStencilDesc.SampleDesc.Count		= 1;
		depthStencilDesc.SampleDesc.Quality	= 0;
	}

	depthStencilDesc.Usage					= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags	= 0;
	depthStencilDesc.MiscFlags			= 0;

	HR( bDevice->CreateTexture2D( &depthStencilDesc, 0, &bDepthStencilBuffer ) );
	HR( bDevice->CreateDepthStencilView( bDepthStencilBuffer, 0, &bDepthStencilView ) );

	bImmediateContext->OMSetRenderTargets( 1, &bRenderTargetView, bDepthStencilView );

	bScreenViewport.TopLeftX	= 0;
	bScreenViewport.TopLeftY	= 0;
	bScreenViewport.Width			= static_cast<float>( bClientWidth );
	bScreenViewport.Height		= static_cast<float>( bClientHeight );
	bScreenViewport.MinDepth	= 0.0f;
	bScreenViewport.MaxDepth	= 1.0f;

	bImmediateContext->RSSetViewports( 1, &bScreenViewport );

	//bObjects.OnResize(bClientWidth, bClientHeight, AspectRatio());
	bLevels.OnResize( bClientWidth, bClientHeight, AspectRatio() );
	bDaisy.OnResize(bClientWidth, bClientHeight, AspectRatio());
	bFreeCam.OnResize(bClientWidth, bClientHeight, AspectRatio());
}

LRESULT GameCore::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message ) {
	case WM_ACTIVATE:
		if( LOWORD( wParam ) == WA_INACTIVE ) 
		{
			bGamePaused = true;
			bTimer.Stop();
		} 
		else 
		{
			bGamePaused = false;
			bTimer.Start();
		}
		return 0;
	case WM_SIZE:
		bClientWidth	= LOWORD( lParam );
		bClientHeight	= HIWORD( lParam );
		if( bDevice ) 
		{
			if( wParam == SIZE_MINIMIZED ) 
			{
				bGamePaused = true;
				bMinimized	= true;
				bMaximized	= false;
			} 
			else if( wParam == SIZE_MAXIMIZED ) 
			{
				bGamePaused	= false;
				bMinimized	= false;
				bMaximized	= true;
				OnResize();
			} 
			else if( wParam == SIZE_RESTORED ) 
			{
				if( bMinimized ) 
				{
					bGamePaused = false;
					bMinimized	= false;
					OnResize();
				} 
				else if( bMaximized ) 
				{
					bGamePaused	= false;
					bMaximized	= false;
					OnResize();
				} 
				else if( bResizing ) 
				{
					// Do Nothing...
				} 
				else 
				{
					OnResize();
				}
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		bGamePaused	= true;
		bResizing		= true;
		bTimer.Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		bGamePaused	=	false;
		bResizing		= false;
		bTimer.Start();
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT( 0, MNC_CLOSE );
	case WM_GETMINMAXINFO:
		( ( MINMAXINFO* )lParam )->ptMinTrackSize.x	= 200;
		( ( MINMAXINFO* )lParam )->ptMinTrackSize.y	= 200;
		return 0;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

bool GameCore::InitWindow()
{
  // Register class
  WNDCLASS wc;
  wc.style					= CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc		= MainWndProc;
  wc.cbClsExtra			= 0;
  wc.cbWndExtra			= 0;
  wc.hInstance			= bGameInst;
  wc.hIcon					= LoadIcon( bGameInst, MAKEINTRESOURCE(IDI_BEAST_ICON) );
  wc.hCursor				= LoadCursor( bGameInst, MAKEINTRESOURCE(IDC_BEAST_CURSOR) );
  wc.hbrBackground	= ( HBRUSH )GetStockObject(NULL_BRUSH);
  wc.lpszMenuName		= NULL;
  wc.lpszClassName	= L"BeastWindowClass";
  if( !RegisterClass( &wc ) ) 
	{
		MessageBox( 0, L"RegisterClass Failed.", 0, 0 );
		return false;
	}

  // Create window
  RECT rc = { 0, 0, bClientWidth, bClientHeight };
  AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, false );
	int width		= rc.right - rc.left;
	int height	= rc.bottom - rc.top;
	bMainWnd = CreateWindow( L"BeastWindowClass", bMainWndCaption.c_str(), WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, bGameInst, 0 );
  if( !bMainWnd )
	{
		MessageBox( 0, L"CreateWindow Failed.", 0, 0 );
    return false;
	}

  ShowWindow( bMainWnd, SW_SHOW );
	UpdateWindow( bMainWnd );

  return true;
}

bool GameCore::InitD3D()
{
	UINT createDeviceFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice( 0, bd3dDriverType, 0, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &bDevice, &featureLevel, &bImmediateContext );

	if( FAILED( hr ) ) {
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if( featureLevel != D3D_FEATURE_LEVEL_11_0 ) {
		MessageBox( 0, L"Direct3D 11 unsuported.", 0, 0 );
		return false;
	}

	HR( bDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &b4xMSAAQuality ) );
	assert( b4xMSAAQuality > 0 );

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width										= bClientWidth;
	sd.BufferDesc.Height									= bClientHeight;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferDesc.Format									= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering				= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling									= DXGI_MODE_SCALING_UNSPECIFIED;

	if( bMsaaLevel == MSAA8X ) 
	{
		sd.SampleDesc.Count		= 8;
		sd.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else if( bMsaaLevel == MSAA4X ) 
	{
		sd.SampleDesc.Count		= 4;
		sd.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else if( bMsaaLevel == MSAA2X ) 
	{
		sd.SampleDesc.Count		= 2;
		sd.SampleDesc.Quality	= b4xMSAAQuality - 1;
	} 
	else 
	{
		sd.SampleDesc.Count		= 1;
		sd.SampleDesc.Quality	= 0;
	}

	sd.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount	= 1;
	sd.OutputWindow	= bMainWnd;
	sd.Windowed			= true;
	sd.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags				= 0;

	IDXGIDevice *dxgiDevice = 0;
	HR( bDevice->QueryInterface ( __uuidof( IDXGIDevice ), ( void** )&dxgiDevice ) );

	IDXGIAdapter *dxgiAdapter = 0;
	HR( dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), ( void** )&dxgiAdapter ) );

	IDXGIFactory *dxgiFactory = 0;
	HR( dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), ( void** )&dxgiFactory ) );

	HR( dxgiFactory->CreateSwapChain( bDevice, &sd, &bSwapChain ) );

	dxgiFactory->MakeWindowAssociation( bMainWnd, DXGI_MWA_NO_WINDOW_CHANGES );

	RELEASE( dxgiDevice );
	RELEASE( dxgiAdapter );
	RELEASE( dxgiFactory );

	OnResize();

	return true;
}

void GameCore::CalculateFrameStats()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if( bTimer.TotalTime() - timeElapsed >= 0.1f ) 
	{
		float fps = ( float )frameCnt / 0.1f;
		float mspf = 1000 / fps;
		
		std::wostringstream outs;
		outs.precision( 6 );
		outs << bMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(bMainWnd, outs.str().c_str());
		
		frameCnt = 0;
		timeElapsed += 0.1f;
	}
}