/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include "Helper.h"

class Camera
{
public:
	Camera( void );
	~Camera( void );

	//--------------------------------------------------
	// Get/Set world camera position.
	//--------------------------------------------------
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	void ClampCamera( float groundHeight );
	
	//--------------------------------------------------
	// Get camera basis vectors.
	//--------------------------------------------------
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;

	//--------------------------------------------------
	// Get frustum properties.
	//--------------------------------------------------
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	//--------------------------------------------------
	// Get near and far plane dimensions in view space coordinates.
	//--------------------------------------------------
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	//--------------------------------------------------
	// Set frustum.
	//--------------------------------------------------
	void SetLens(float FovY, float Aspect, float Zn, float Zf);

	//--------------------------------------------------
	// Define camera space via LookAt parameters.
	//--------------------------------------------------
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	//--------------------------------------------------
	// Get View/Proj matrices.
	//--------------------------------------------------
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	//--------------------------------------------------
	// Strafe/Walk the camera a distance d.
	//--------------------------------------------------
	void Strafe(float d);
	void Walk(float d);

	//--------------------------------------------------
	// Rotate the camera.
	//--------------------------------------------------
	void Pitch(float angle);
	void RotateY(float angle);

	//--------------------------------------------------
	// After modifying camera position/orientation, call to rebuild the view matrix.
	//--------------------------------------------------
	void UpdateViewMatrix();

private:
	//--------------------------------------------------
	// Camera coordinate system with coordinates relative to world space.
	//--------------------------------------------------
	XMFLOAT3 position;
	XMFLOAT3 right;
	XMFLOAT3 up;
	XMFLOAT3 look;

	//--------------------------------------------------
	// Cache frustum properties.
	//--------------------------------------------------
	float nearZ;
	float farZ;
	float aspect;
	float fovY;
	float nearWindowHeight;
	float farWindowHeight;

	//--------------------------------------------------
	// Cache View/Proj matrices.
	//--------------------------------------------------
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
};

#endif // CAMERA_H