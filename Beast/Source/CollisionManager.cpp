#include "CollisionManager.h"
/*
void CollisionManager::Init(ID3D11Device *device)
{
}

void CollisionManager::Destroy(void)
{

}*/
/*
std::vector<SkinnedModelInstance *> CollisionManager::Animated;
std::vector<BasicModelInstance *> CollisionManager::Static;
std::vector<Level *> CollisionManager::Levels;

std::vector<std::pair<UINT, UINT>> CollisionManager::CollisionsAnimStatic;
std::vector<std::pair<UINT, UINT>> CollisionManager::CollisionsAnimAnim;

void CollisionManager::Draw(ID3D11Device *device, ID3D11DeviceContext *context, Camera* cam)
{
	context->IASetInputLayout(InputLayouts::Basic);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	UINT stride = sizeof(Vertex::Basic);
	UINT offset = 0;

	XMMATRIX view = cam->View();
	XMMATRIX proj = cam->Proj();
	XMMATRIX viewProj = cam->ViewProj();

	ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light3TexTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	Material Mat;
	Mat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Mat.Diffuse = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	Mat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);

	for (UINT i = 0; i < Animated.size(); ++i)
	{
		for (UINT j = 0; j < Animated[i]->Model->Collider.OBBs.size(); ++j)
		{
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				context->IASetVertexBuffers(0, 1, &Animated[i]->Model->Collider.OBBs[j].second.VB, &stride, &offset);
				context->IASetIndexBuffer(Animated[i]->Model->Collider.OBBs[j].second.IB, DXGI_FORMAT_R32_UINT, 0);

				XMMATRIX world = XMMatrixRotationQuaternion(XMLoadFloat4(&Animated[i]->Model->Collider.OBBs[j].second.OBB.Orientation)) *
					XMLoadFloat4x4(&Animated[i]->World) *
					XMLoadFloat4x4(&Animated[i]->FinalTransforms[Animated[i]->Model->Collider.OBBs[j].first]);
				XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
				XMMATRIX worldViewProj = world*view*proj;

				Effects::BasicFX->SetWorld(world);
				Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
				Effects::BasicFX->SetWorldViewProj(worldViewProj);
				//Effects::BasicFX->SetDiffuseMap(texture->GetTexture());
				//Effects::BasicFX->SetTexTransform(XM);
				Effects::BasicFX->SetMaterial(Mat);

				activeTech->GetPassByIndex(p)->Apply(0, context);
				context->DrawIndexed(24, 0, 0);
			}
		}
	}

	for (UINT i = 0; i < Static.size(); ++i)
	{
		for (UINT j = 0; j < Static[i]->Model->Collider.OBBs.size(); ++j)
		{
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				context->IASetVertexBuffers(0, 1, &Static[i]->Model->Collider.OBBs[j].second.VB, &stride, &offset);
				context->IASetIndexBuffer(Static[i]->Model->Collider.OBBs[j].second.IB, DXGI_FORMAT_R32_UINT, 0);

				XMMATRIX world = XMMatrixRotationQuaternion(XMLoadFloat4(&Static[i]->Model->Collider.OBBs[j].second.OBB.Orientation));
				XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
				XMMATRIX worldViewProj = world*view*proj;

				Effects::BasicFX->SetWorld(world);
				Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
				Effects::BasicFX->SetWorldViewProj(worldViewProj);
				//Effects::BasicFX->SetDiffuseMap(texture->GetTexture());
				//Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&TexTransform));
				Effects::BasicFX->SetMaterial(Mat);

				activeTech->GetPassByIndex(p)->Apply(0, context);
				context->DrawIndexed(24, 0, 0);
			}
		}
	}
	
	for (UINT i = 0; i < Levels.size(); ++i)
	{
		for (UINT j = 0; j < Levels[i]->Collider.OBBs.size(); ++j)
		{
			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				context->IASetVertexBuffers(0, 1, &Levels[i]->Collider.OBBs[j].second.VB, &stride, &offset);
				context->IASetIndexBuffer(Levels[i]->Collider.OBBs[j].second.IB, DXGI_FORMAT_R32_UINT, 0);

				XMMATRIX world = XMMatrixRotationQuaternion(XMLoadFloat4(&Levels[i]->Collider.OBBs[j].second.OBB.Orientation));
				XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
				XMMATRIX worldViewProj = world*view*proj;

				Effects::BasicFX->SetWorld(world);
				Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
				Effects::BasicFX->SetWorldViewProj(worldViewProj);
				//Effects::BasicFX->SetDiffuseMap(texture->GetTexture());
				//Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&TexTransform));
				Effects::BasicFX->SetMaterial(Mat);

				activeTech->GetPassByIndex(p)->Apply(0, context);
				context->DrawIndexed(24, 0, 0);
			}
		}
	}
}

void CollisionManager::CheckCollisions()
{

}*/
