#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include "Character.h"
#include "TextManager.h"


Character::Character()
{
	//aCam = new Camera();
}


Character::~Character()
{
	//DEL(aCam);

	//aDIKeyboard->Unacquire();
	//aDIMouse->Unacquire();
	//RELEASE(aDirectInput);
}

bool Character::Init(ID3D11Device *device, ID3D11DeviceContext *context, IDXGISwapChain *sc, HINSTANCE hInstance, HWND hwnd, LevelManager *lm, ObjectManager *om)
{
	//if (!InitDirectInput(hInstance, hwnd))
		//return false;

	aObjManager = om;
	aLevelManager = lm;

	//aCam->SetPosition(0.0f, 0.0f, 0.0f);
	/*
	aCharInst.Model = aObjManager->GetSkinned("Daisy");
	aCharInst.defaultClipName = "idle";
	aCharInst.Model->SkinnedData.SetCurrAnim(aCharInst.defaultClipName);
	aCharInst.TimePos = 0.0f;
	aCharInst.FinalTransforms.resize(aCharInst.Model->SkinnedData.BoneCount());
	aCharInst.position.x = 0.0f;
	aCharInst.position.y = 0.0f;//terrain.GetHeight(2.0f, -7.0f); // TODO!!
	aCharInst.position.z = 0.0f;
	aCharInst.fwd = aCam->GetLook();
	aCharInst.right = aCam->GetRight();
	aCharInst.up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&aCharInst.rot, XMMatrixIdentity()); //XMMatrixRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), XM_PI));
	aCharInst.scale.x = 1.0f;
	aCharInst.scale.y = 1.0f;
	aCharInst.scale.z = 1.0f;*/

	/*XMVECTOR s = XMVectorReplicate(	8.0f);
	XMVECTOR l = XMLoadFloat3(&aCam->GetLook());
	XMVECTOR p = XMLoadFloat3(&aCam->GetPosition());
	XMStoreFloat3(&aCharInst.position, XMVectorMultiplyAdd(s, l, p));*/

	//aCharInst.Update(0.0f);

	/*CollisionManager::Animated.push_back(&aCharInst);
	myColiderPos = CollisionManager::Animated.size() - 1;*/

	aModInst.Model = aObjManager->GetStatic("Daisy");
	aModInst.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	aModInst.scale = XMFLOAT3(0.3f, 0.3f, -0.3f);
	/*aModInst.fwd = aCam->GetLook();
	aModInst.right = aCam->GetRight();
	aModInst.up = XMFLOAT3(0.0f, 1.0f, 0.0f);*/

	XMMATRIX modelScale = XMMatrixScaling(0.02f, 0.02f, -0.02f);
	XMMATRIX modelRot = XMMatrixRotationY(0);// MathHelper::Pi);
	XMMATRIX modelOffset = XMMatrixTranslation(-2.0f, 0.0f, -7.0f);
	XMStoreFloat4x4(&aModInst.World, modelScale*modelRot*modelOffset);

	aModInst.Update(0.0f);

	std::wstring txt = L"Char: X: " + std::to_wstring((float)aModInst.position.x) + L" Y: " + std::to_wstring((float)aModInst.position.y) + L" Z: " + std::to_wstring((float)aModInst.position.z);
	TextManager::texts.push_back(txt);
	charPos = TextManager::texts.size() - 1;

	//aLevelManager->SetCamera(aCam);

	return true;
}

bool Character::InitDirectInput(HINSTANCE hInst, HWND hwnd)
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

void Character::Update(float dt)
{
	//aCharInst.Update(dt);

	//aCam->ClampCamera(5.0f);
	//aCam->UpdateViewMatrix();

	//TextManager::texts[charPos] = L"Char: X: " + std::to_wstring((float)aCharInst.position.x) + L" Y: " + std::to_wstring((float)aCharInst.position.y) + L" Z: " + std::to_wstring((float)aCharInst.position.z);
	//TextManager::texts[camPos] = L"Cam: X: " + std::to_wstring((float)aCam->GetPosition().x) + L" Y: " + std::to_wstring((float)aCam->GetPosition().y) + L" Z: " + std::to_wstring((float)aCam->GetPosition().z);
}

void Character::Draw(ID3D11Device *device, ID3D11DeviceContext *context)
{
	aModInst.Draw(device, context, aCam, aLevelManager->GetLevel()->GetDirLights(), aLevelManager->GetLevel()->GetSky());
}

void Character::OnResize(int width, int height, float aspect)
{
	//aCam->SetLens(0.25f*MathHelper::Pi, aspect, 1.0f, 1000.0f);
}

void Character::DetectInput(float dt, HWND hwnd)
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
		//aModelInst1.isWalking = true;
		aCharInst.Walk(moveSpeed * dt, aCam); // TODO!!
		aCam->Walk(moveSpeed * dt);
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		//aModelInst1.isWalking = true;
		aCharInst.Walk(-moveSpeed * dt, aCam/*, &terrain*/); // TODO!!
		aCam->Walk(-moveSpeed * dt);
	}
	if (keyboardState[DIK_A] & 0x80)
	{
		//aModelInst1.isWalking = true;
		aCharInst.Strafe(-moveSpeed * dt, aCam/*, &terrain*/); // TODO!!
		aCam->Strafe(-moveSpeed * dt);
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		//aModelInst1.isWalking = true;
		aCharInst.Strafe(moveSpeed * dt, aCam/*, &terrain*/); // TODO!!
		aCam->Strafe(moveSpeed * dt);
	}

	if (currMouseState.lX != aMouseLastState.lX)
	{
		aCam->SetPosition(aCharInst.position);
		aCam->RotateY(XMConvertToRadians(0.25f * static_cast<float>(currMouseState.lX)));
		XMFLOAT3 camPos;
		camPos.x = aCharInst.position.x - (8 * aCam->GetLook().x);
		camPos.y = aCharInst.position.y - (8 * aCam->GetLook().y);
		camPos.z = aCharInst.position.z - (8 * aCam->GetLook().z);
		aCam->SetPosition(camPos);

		aCharInst.RotateY(XMConvertToRadians(0.25f * static_cast<float>(currMouseState.lX)));
	}

	if (currMouseState.lY != aMouseLastState.lY)
	{
		aCam->SetPosition(aCharInst.position);
		aCam->Pitch(XMConvertToRadians(0.12f * static_cast<float>(currMouseState.lY)), true);
		XMFLOAT3 camPos;
		camPos.x = aCharInst.position.x - (8 * aCam->GetLook().x);
		camPos.y = aCharInst.position.y - (8 * aCam->GetLook().y);
		camPos.z = aCharInst.position.z - (8 * aCam->GetLook().z);
		aCam->SetPosition(camPos);
	}

	aMouseLastState = currMouseState;

	return;
}

Camera *Character::GetCam()
{
	return aCam;
}

void Character::SetCam(Camera *cam)
{
	aCam = cam;
}
