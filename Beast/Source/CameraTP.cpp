/*************************************************************************************
* File: Init.h; Date of creation: 11.Feb.2015; Date of Last Modification: 11.Feb.2015
*
* This is a header file to helper stuff.
*
* Copyright (c) Dominik Horòák. All rights reserved.
**************************************************************************************/
#include "CameraTP.h"

Camera::Camera()
	: position(0.0f, 0.0f, 0.0f), 
	  right(1.0f, 0.0f, 0.0f),
	  up(0.0f, 1.0f, 0.0f),
	  look(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f*MathHelper::Pi, 1.0f, 1.0f, 1000.0f);
}

Camera::~Camera()
{
}

XMVECTOR Camera::GetPositionXM()const
{
	return XMLoadFloat3(&position);
}

XMFLOAT3 Camera::GetPosition()const
{
	return position;
}

void Camera::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

void Camera::SetPosition(const XMFLOAT3& v)
{
	position = v;
}

void Camera::ClampCamera( float groundHeight )
{
	position.y = groundHeight + 4.0f;
}

XMVECTOR Camera::GetRightXM()const
{
	return XMLoadFloat3(&right);
}

XMFLOAT3 Camera::GetRight()const
{
	return right;
}

XMVECTOR Camera::GetUpXM()const
{
	return XMLoadFloat3(&up);
}

XMFLOAT3 Camera::GetUp()const
{
	return up;
}

XMVECTOR Camera::GetLookXM()const
{
	return XMLoadFloat3(&look);
}

XMFLOAT3 Camera::GetLook()const
{
	return look;
}

float Camera::GetNearZ()const
{
	return nearZ;
}

float Camera::GetFarZ()const
{
	return farZ;
}

float Camera::GetAspect()const
{
	return aspect;
}

float Camera::GetFovY()const
{
	return fovY;
}

float Camera::GetFovX()const
{
	float halfWidth = 0.5f*GetNearWindowWidth();
	return 2.0f*atan(halfWidth / nearZ);
}

float Camera::GetNearWindowWidth()const
{
	return aspect * nearWindowHeight;
}

float Camera::GetNearWindowHeight()const
{
	return nearWindowHeight;
}

float Camera::GetFarWindowWidth()const
{
	return aspect * farWindowHeight;
}

float Camera::GetFarWindowHeight()const
{
	return farWindowHeight;
}

void Camera::SetLens(float FovY, float Aspect, float Zn, float Zf)
{
	// cache properties
	fovY = FovY;
	aspect = Aspect;
	nearZ = Zn;
	farZ = Zf;

	nearWindowHeight = 2.0f * nearZ * tanf( 0.5f*fovY );
	farWindowHeight  = 2.0f * farZ * tanf( 0.5f*fovY );

	XMMATRIX P = XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	XMStoreFloat4x4(&proj, P);
}

void Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&position, pos);
	XMStoreFloat3(&look, L);
	XMStoreFloat3(&right, R);
	XMStoreFloat3(&up, U);
}

void Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX Camera::View()const
{
	return XMLoadFloat4x4(&view);
}

XMMATRIX Camera::Proj()const
{
	return XMLoadFloat4x4(&proj);
}

XMMATRIX Camera::ViewProj()const
{
	return XMMatrixMultiply(View(), Proj());
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&right);
	XMVECTOR p = XMLoadFloat3(&position);
	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, r, p));
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&look);
	XMVECTOR p = XMLoadFloat3(&position);
	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, l, p));
}

void Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&right), angle);

	XMStoreFloat3(&up,   XMVector3TransformNormal(XMLoadFloat3(&up), R));
	XMStoreFloat3(&look, XMVector3TransformNormal(XMLoadFloat3(&look), R));
}

void Camera::RotateY(float angle) 
{
	// Rotate the basis vectors about the world y-axis.
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&right,   XMVector3TransformNormal(XMLoadFloat3(&right), R));
	XMStoreFloat3(&up, XMVector3TransformNormal(XMLoadFloat3(&up), R));
	XMStoreFloat3(&look, XMVector3TransformNormal(XMLoadFloat3(&look), R));
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&right);
	XMVECTOR U = XMLoadFloat3(&up);
	XMVECTOR L = XMLoadFloat3(&look);
	XMVECTOR P = XMLoadFloat3(&position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L); 

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&right, R);
	XMStoreFloat3(&up, U);
	XMStoreFloat3(&look, L);

	view(0,0) = right.x; 
	view(1,0) = right.y; 
	view(2,0) = right.z; 
	view(3,0) = x;   

	view(0,1) = up.x;
	view(1,1) = up.y;
	view(2,1) = up.z;
	view(3,1) = y;  

	view(0,2) = look.x; 
	view(1,2) = look.y; 
	view(2,2) = look.z; 
	view(3,2) = z;   

	view(0,3) = 0.0f;
	view(1,3) = 0.0f;
	view(2,3) = 0.0f;
	view(3,3) = 1.0f;
}
