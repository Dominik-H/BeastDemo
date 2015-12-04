#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include "FreeCam.h"
#include "TextManager.h"


FreeCam::FreeCam()
{
	aCam = new Camera();
}


FreeCam::~FreeCam()
{
	DEL(aCam);
	aDIKeyboard->Unacquire();
	aDIMouse->Unacquire();
	RELEASE(aDirectInput);
}

bool FreeCam::Init(ID3D11Device *device, ID3D11DeviceContext *context, IDXGISwapChain *sc, HINSTANCE hInstance, HWND hwnd, LevelManager *lm, ObjectManager *om)
{
	if (!InitDirectInput(hInstance, hwnd))
		return false;

	aObjManager = om;
	aLevelManager = lm;

	aCam->SetPosition(0.0f, 2.0f, 0.0f);

	std::wstring txt = L"Cam: X: " + std::to_wstring((float)aCam->GetPosition().x) + L" Y: " + std::to_wstring((float)aCam->GetPosition().y) + L" Z: " + std::to_wstring((float)aCam->GetPosition().z);
	TextManager::texts.push_back(txt);
	camPos = TextManager::texts.size() - 1;

	aLevelManager->SetCamera(aCam);

	return true;
}

bool FreeCam::InitDirectInput(HINSTANCE hInst, HWND hwnd)
{
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&aDirectInput, NULL);

	HR(aDirectInput->CreateDevice(GUID_SysKeyboard, &aDIKeyboard, NULL));
	HR(aDirectInput->CreateDevice(GUID_SysMouse, &aDIMouse, NULL));

	HR(aDIKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(aDIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));

	HR(aDIMouse->SetDataFormat(&c_dfDIMouse));
	HR(aDIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND));

	return true;
}

void FreeCam::Update(float dt)
{
	//aCam.ClampCamera(5.0f);
	aCam->UpdateViewMatrix();

	TextManager::texts[camPos] = L"Cam: X: " + std::to_wstring((float)aCam->GetPosition().x) + L" Y: " + std::to_wstring((float)aCam->GetPosition().y) + L" Z: " + std::to_wstring((float)aCam->GetPosition().z);
}

void FreeCam::OnResize(int width, int height, float aspect)
{
	aCam->SetLens(0.25f*MathHelper::Pi, aspect, 1.0f, 1000.0f);
}

void FreeCam::DetectInput(float dt, HWND hwnd)
{
	float moveSpeed = 25.0f;

	DIMOUSESTATE currMouseState;
	BYTE keyboardState[256];

	aDIKeyboard->Acquire();
	aDIMouse->Acquire();

	aDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &currMouseState);
	aDIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	if (keyboardState[DIK_W] & 0x80)
	{
		aCam->Walk(moveSpeed * dt);
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		aCam->Walk(-moveSpeed * dt);
	}
	if (keyboardState[DIK_A] & 0x80)
	{
		aCam->Strafe(-moveSpeed * dt);
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		aCam->Strafe(moveSpeed * dt);
	}

	if (currMouseState.lX != aMouseLastState.lX)
	{
		//aCam.SetPosition(aCharInst.position);
		aCam->RotateY(XMConvertToRadians(0.25f * static_cast<float>(currMouseState.lX)));
		/*XMFLOAT3 camPos;
		camPos.x = aCharInst.position.x - (8 * aCam.GetLook().x);
		camPos.y = aCharInst.position.y - (8 * aCam.GetLook().y);
		camPos.z = aCharInst.position.z - (8 * aCam.GetLook().z);
		aCam.SetPosition(camPos);

		aCharInst.RotateY(XMConvertToRadians(0.25f * static_cast<float>(currMouseState.lX)));*/
	}

	if (currMouseState.lY != aMouseLastState.lY)
	{
		//aCam.SetPosition(aCharInst.position);
		aCam->Pitch(XMConvertToRadians(0.12f * static_cast<float>(currMouseState.lY)), false);
		/*XMFLOAT3 camPos;
		camPos.x = aCharInst.position.x - (8 * aCam.GetLook().x);
		camPos.y = aCharInst.position.y - (8 * aCam.GetLook().y);
		camPos.z = aCharInst.position.z - (8 * aCam.GetLook().z);
		aCam.SetPosition(camPos);*/
	}

	aMouseLastState = currMouseState;

	return;
}

Camera *FreeCam::GetCam()
{
	return aCam;
}

void FreeCam::SetCam(Camera *cam)
{
	aCam = cam;
}

