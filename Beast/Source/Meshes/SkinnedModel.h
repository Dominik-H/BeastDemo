#ifndef SKINNEDMODEL_H
#define SKINNEDMODEL_H

#include "SkinnedData.h"
#include "MeshGeometry.h"
#include "..\TextureMgr.h"
#include "..\Vertex.h"
#include "..\Structs.h"
#include "../Camera.h"
#include "../Objects/Terrain.h"
#include "../Objects/Skybox.h"
#include "../Collider.h"
#include "../cAnimationController.h"

class SkinnedModel
{
public:
	SkinnedModel();

	SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, std::string& modelFilename, const std::wstring& texturePath);
	~SkinnedModel();

	void AddAnim(std::string& filename);

	UINT SubsetCount;

	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;
	std::vector<ID3D11ShaderResourceView*> NormalMapSRV;

	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormalTexTanSkinned> Vertices;
	std::vector<USHORT> Indices;
	std::vector<MeshGeometry::Subset> Subsets;

	MeshGeometry ModelMesh;
	SkinnedData SkinnedData;
	SceneAnimator animations;
};

struct SkinnedModelInstance
{
	//SkinnedModelInstance(SkinnedModel* model, std::string defClipName);

	SkinnedModel* Model;
	float TimePos;
	std::string defaultClipName; // Should be sort of Idle animation
	bool isWalking;

	XMFLOAT3 position;
	XMFLOAT3 up;
	XMFLOAT3 fwd;
	XMFLOAT3 right;
	XMFLOAT4X4 rot;
	XMFLOAT3 scale;

	XMFLOAT4X4 World;
	std::vector<XMFLOAT4X4> FinalTransforms;

	void Walk(float dt, Camera* cam/*, Terrain* ter*/);
	void Strafe( float dt, Camera* cam/*, Terrain* ter*/ );
	void RotateY( float angle );
	void Update(float dt);
	void Draw(ID3D11Device *device, ID3D11DeviceContext *context, Camera *cam, DirectionalLight dirLights[3], Skybox *sky);
	XMFLOAT3 GetPos();
};

#endif // SKINNEDMODEL_H