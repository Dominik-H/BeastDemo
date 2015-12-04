#include "BasicModel.h"
#include "../Effects.h"
#include "../Objects/Skybox.h"
#include "../ModelLoader.h"
#include <locale>
#include <codecvt>

BasicModel::BasicModel()
{

}

BasicModel::BasicModel(ID3D11Device* device, TextureMgr& texMgr, std::string& modelFilename, const std::wstring& texturePath, bool bump)
{
	this->bump = bump;

	std::vector<ExtendedMaterial> mats;
	ModelLoader loader;

	if (bump)
	{
		loader.LoadModelStaticBump(modelFilename, VerticesBump, Indices, Subsets, mats);
		ModelMesh.SetVertices(device, &VerticesBump[0], VerticesBump.size());
	}
	else {
		loader.LoadModelStaticSimp(modelFilename, VerticesSimple, Indices, Subsets, mats);
		ModelMesh.SetVertices(device, &VerticesSimple[0], VerticesSimple.size());
	}

	ModelMesh.SetIndices(device, &Indices[0], Indices.size());
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = Subsets.size();

	// wstr <-> str converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	for (UINT i = 0; i < mats.size(); ++i)
	{
		Material m;
		m.Ambient = mats[i].Ambient;
		m.Diffuse = mats[i].Diffuse;
		m.Reflect = mats[i].Reflect;
		m.Specular = mats[i].Specular;
		Mat.push_back(m);

		if (!mats[i].diffuseMapName.empty())
		{
			int idx = mats[i].diffuseMapName.find_last_of("\\/");
			if (idx == std::string::npos)
				idx = -1;
			std::wstring name = converter.from_bytes(mats[i].diffuseMapName.substr(idx + 1));
			std::wstring p = texturePath + name;

			/*std::wstring suffix = p.substr(p.length() - 3);
			if (suffix != L"dds" || suffix != L"DDS")
			{
			p[p.length() - 3] = L'd';
			p[p.length() - 2] = L'd';
			p[p.length() - 1] = L's';
			}*/

			ID3D11ShaderResourceView* srv;
			srv = texMgr.CreateTexture(p);
			DiffuseMapSRV.push_back(srv);
		}
		else
		{
			ID3D11ShaderResourceView* srv;
			srv = texMgr.CreateTexture(L"Textures/DefaultDiffuse.png");
			DiffuseMapSRV.push_back(srv);
		}

		if (!mats[i].normalMapName.empty())
		{
			int idx = mats[i].normalMapName.find_last_of("\\/");
			if (idx == std::string::npos)
				idx = -1;
			std::wstring name = converter.from_bytes(mats[i].normalMapName.substr(idx + 1));
			std::wstring p = texturePath + name;

			/*std::wstring suffix = p.substr(p.length() - 3);
			if (suffix != L"dds" || suffix != L"DDS")
			{
			p[p.length() - 3] = L'd';
			p[p.length() - 2] = L'd';
			p[p.length() - 1] = L's';
			}*/

			ID3D11ShaderResourceView* srv;
			srv = texMgr.CreateTexture(p);
			NormalMapSRV.push_back(srv);
		}
		else
		{
			ID3D11ShaderResourceView* srv;
			srv = texMgr.CreateTexture(L"Textures/Neutral_normal_map.jpg");
			NormalMapSRV.push_back(srv);
		}
	}
}

BasicModel::~BasicModel()
{
}

void BasicModelInstance::Draw(ID3D11Device *device, ID3D11DeviceContext *context, Camera *cam, DirectionalLight dirLights[3], Skybox *sky)
{
	XMMATRIX view = cam->View();
	XMMATRIX proj = cam->Proj();
	XMMATRIX viewProj = cam->ViewProj();

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	if (Model->bump) {
		Effects::NormalMapFX->SetDirLights(dirLights);
		Effects::NormalMapFX->SetEyePosW(cam->GetPosition());
		Effects::NormalMapFX->SetCubeMap(sky->SkyboxSRV());
	}
	else {
		Effects::BasicFX->SetDirLights(dirLights);
		Effects::BasicFX->SetEyePosW(cam->GetPosition());
		Effects::BasicFX->SetCubeMap(sky->SkyboxSRV());
	}

	if (Model->bump)
		context->IASetInputLayout(InputLayouts::PosNormalTexTan);
	else
		context->IASetInputLayout(InputLayouts::Basic);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* tech;
	if (Model->bump) {
		tech = Effects::NormalMapFX->Light3TexTech;
	}
	else {
		tech = Effects::BasicFX->Light3TexTech;
	}

	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX world = XMLoadFloat4x4(&World);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;

		if (!Model->bump) {
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetWorldViewProjTex(worldViewProj*toTexSpace);
			//Effects::BasicFX->SetShadowTransform(world*shadowTransform);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

			for (UINT subset = 0; subset < Model->SubsetCount; ++subset)
			{
			Effects::BasicFX->SetMaterial(Model->Mat[subset]);
			Effects::BasicFX->SetDiffuseMap(Model->DiffuseMapSRV[subset]);
			//Effects::BasicFX->SetNormalMap(Model->NormalMapSRV[subset]);

			tech->GetPassByIndex(p)->Apply(0, context);
			Model->ModelMesh.Draw(context, subset);
			}
		}
		else {
			Effects::NormalMapFX->SetWorld(world);
			Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
			Effects::NormalMapFX->SetWorldViewProjTex(worldViewProj *toTexSpace);
			//Effects::NormalMapFX->SetShadowTransform(world*shadowTransform);
			Effects::NormalMapFX->SetTexTransform(XMMatrixIdentity());
			//Effects::NormalMapFX->SetBoneTransforms(&FinalTransforms[0], FinalTransforms.size());

			for (UINT subset = 0; subset < Model->SubsetCount; ++subset)
			{
				Effects::NormalMapFX->SetMaterial(Model->Mat[subset]);
				Effects::NormalMapFX->SetDiffuseMap(Model->DiffuseMapSRV[subset]);
				Effects::NormalMapFX->SetNormalMap(Model->NormalMapSRV[subset]);

				tech->GetPassByIndex(p)->Apply(0, context);
				Model->ModelMesh.Draw(context, subset);
			}
		}
	}
}

void BasicModelInstance::Walk(float dt, Camera* cam/*, Terrain* ter*/)
{
	XMVECTOR s = XMVectorReplicate(dt);
	XMVECTOR l = XMLoadFloat3(&cam->GetLook());
	XMVECTOR p = XMLoadFloat3(&position);
	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, l, p));

	position.y = 5.0f;//ter->GetHeight(position.x, position.z);
}

void BasicModelInstance::Strafe(float dt, Camera* cam/*, Terrain* ter*/)
{
	XMVECTOR s = XMVectorReplicate(dt);
	XMVECTOR r = XMLoadFloat3(&cam->GetRight());
	XMVECTOR p = XMLoadFloat3(&position);
	XMStoreFloat3(&position, XMVectorMultiplyAdd(s, r, p));

	position.y = 5.0f;//ter->GetHeight(position.x, position.z);
}


void BasicModelInstance::RotateY(float angle)
{
	XMMATRIX A = XMMatrixRotationY(-angle);
	XMStoreFloat3(&right, XMVector3TransformNormal(XMLoadFloat3(&right), A));
	XMStoreFloat3(&up, XMVector3TransformNormal(XMLoadFloat3(&up), A));
	XMStoreFloat3(&fwd, XMVector3TransformNormal(XMLoadFloat3(&fwd), A));

	XMStoreFloat4x4(&rot, XMMatrixIdentity());

	rot(0, 0) = right.x;
	rot(1, 0) = right.y;
	rot(2, 0) = right.z;

	rot(0, 1) = up.x;
	rot(1, 1) = up.y;
	rot(2, 1) = up.z;

	rot(0, 2) = fwd.x;
	rot(1, 2) = fwd.y;
	rot(2, 2) = fwd.z;
}

void BasicModelInstance::Update(float dt)
{
	/*XMMATRIX modelScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX modelOffset = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX Rot = XMLoadFloat4x4(&rot);
	XMStoreFloat4x4(&World, modelScale*Rot*modelOffset);

	if (dt < 0)
		dt = -dt;*/

	/*
	TimePos += dt;
	Model->SkinnedData.GetFinalTransforms(TimePos, FinalTransforms);
	Model->SkinnedData.nextTimePos += dt;

	// Loop animation
	if (TimePos > Model->SkinnedData.GetClipEndTime())
		TimePos = 0.0f;*/
}

XMFLOAT3 BasicModelInstance::GetPos()
{
	return position;
}
