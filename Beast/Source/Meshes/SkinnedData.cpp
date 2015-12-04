

#include "SkinnedData.h"
#include "MeshGeometry.h"

Keyframe::Keyframe()
	: TimePos(0.0f),
	Translation(0.0f, 0.0f, 0.0f),
	Scale(1.0f, 1.0f, 1.0f),
	RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{
}

Keyframe::~Keyframe()
{
}
 
float BoneAnimation::GetStartTime()const
{
	// Keyframes are sorted by time, so first keyframe gives start time.
	return Keyframes.front().TimePos;
}

float BoneAnimation::GetEndTime()const
{
	// Keyframes are sorted by time, so last keyframe gives end time.
	float f = Keyframes.back().TimePos;

	return f;
}

void BoneAnimation::Interpolate(float t, XMFLOAT3& S, XMFLOAT3& P, XMFLOAT4& Q)const
{
	if( t <= Keyframes.front().TimePos )
	{
		S = Keyframes.front().Scale;
		P = Keyframes.front().Translation;
		Q = Keyframes.front().RotationQuat;

		/*XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));*/
	}
	else if( t >= Keyframes.back().TimePos )
	{
		S = Keyframes.back().Scale;
		P = Keyframes.back().Translation;
		Q = Keyframes.back().RotationQuat;

		/*XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));*/
	}
	else
	{
		for(UINT i = 0; i < Keyframes.size()-1; ++i)
		{
			if( t >= Keyframes[i].TimePos && t <= Keyframes[i+1].TimePos )
			{
				float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i+1].TimePos - Keyframes[i].TimePos);

				XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
				XMVECTOR s1 = XMLoadFloat3(&Keyframes[i+1].Scale);

				XMVECTOR p0 = XMLoadFloat3(&Keyframes[i].Translation);
				XMVECTOR p1 = XMLoadFloat3(&Keyframes[i+1].Translation);

				XMVECTOR q0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
				XMVECTOR q1 = XMLoadFloat4(&Keyframes[i+1].RotationQuat);

				XMStoreFloat3(&S, XMVectorLerp(s0, s1, lerpPercent));
				XMStoreFloat3(&P, XMVectorLerp(p0, p1, lerpPercent));
				XMStoreFloat4(&Q, XMQuaternionSlerp(q0, q1, lerpPercent));

				/*XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
				XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));*/

				break;
			}
		}
	}
}

float AnimationClip::GetClipStartTime()const
{
	// Find smallest start time over all bones in this clip.
	float t = MathHelper::Infinity;
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
	}

	return t;
}

float AnimationClip::GetClipEndTime()const
{
	// Find largest end time over all bones in this clip.
	float t = 0.0f;
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
	}

	return t;
}

void AnimationClip::Interpolate(float t, std::vector<XMFLOAT3>& STransforms, std::vector<XMFLOAT3>& PTransforms, std::vector<XMFLOAT4>& QTransforms)const
{
	for(UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		BoneAnimations[i].Interpolate(t, STransforms[i], PTransforms[i], QTransforms[i]);
	}
}

SkinnedData::SkinnedData()
{

}

float SkinnedData::GetClipStartTime()const
{
	auto clip = mAnimations.find(currAnimName);
	return clip->second.GetClipStartTime();
}

float SkinnedData::GetClipEndTime()const
{
	auto clip = mAnimations.find(currAnimName);
	return clip->second.GetClipEndTime();
}

UINT SkinnedData::BoneCount()const
{
	return mBoneHierarchy.size();
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, 
					  std::vector<std::string>& boneNameHierarchy,
		              std::vector<XMFLOAT4X4>& boneOffsets,
		              std::map<std::string, AnimationClip>& animations)
{
	mBoneHierarchy = boneHierarchy;
	mBoneNameHierarchy = boneNameHierarchy;
	mBoneOffsets   = boneOffsets;
	mAnimations    = animations;
}
 
bool SkinnedData::GetFinalTransforms(float timePos,  std::vector<XMFLOAT4X4>& finalTransforms)
{
	if (currAnimName.empty())
		return false;

	UINT numBones = mBoneOffsets.size();

	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	// Interpolate all the bones of this clip at the given time instance.
	//auto clip = mAnimations.find(clipName);
	//clip->second.Interpolate(timePos, toParentTransforms);
	BlendAnims(timePos, toParentTransforms);

	//
	// Traverse the hierarchy and transform all the bones to the root space.
	//

	std::vector<XMFLOAT4X4> toRootTransforms(numBones);

	// The root bone has index 0.  The root bone has no parent, so its toRootTransform
	// is just its local bone transform.
	toRootTransforms[0] = toParentTransforms[0];

	// Now find the toRootTransform of the children.
	for(UINT i = 1; i < numBones; ++i)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);

		int parentIndex = mBoneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);

		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);

		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	// Premultiply by the bone offset transform to get the final transform.
	for(UINT i = 0; i < numBones; ++i)
	{
		XMMATRIX offset = XMLoadFloat4x4(&mBoneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset,toRoot));
	}

	return true;
}

void SkinnedData::SetNextAnim(std::string& clipName)
{
	nextAnimName = clipName;
	nextTimePos = 0.0f;
	blendFactor = 0.0f;
}

void SkinnedData::SetCurrAnim(std::string& clipName)
{
	currAnimName = clipName;
}

void SkinnedData::BlendAnims(float timePos, std::vector<XMFLOAT4X4>& finalTransforms)
{
	if (nextAnimName.empty())
	{
		UINT numBones = mBoneOffsets.size();

		std::vector<XMFLOAT3> S(numBones);
		std::vector<XMFLOAT3> P(numBones);
		std::vector<XMFLOAT4> Q(numBones);

		auto clip = mAnimations.find(currAnimName);
		clip->second.Interpolate(timePos, S, P, Q);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		for (UINT i = 0; i < numBones; ++i)
		{
			XMStoreFloat4x4(&finalTransforms[i], XMMatrixAffineTransformation(XMLoadFloat3(&S[i]), zero, XMLoadFloat4(&Q[i]), XMLoadFloat3(&P[i])));
		}
	}
	else {
		UINT numBones = mBoneOffsets.size();

		std::vector<XMFLOAT3> S0(numBones);
		std::vector<XMFLOAT3> P0(numBones);
		std::vector<XMFLOAT4> Q0(numBones);

		auto clip0 = mAnimations.find(currAnimName);
		clip0->second.Interpolate(timePos, S0, P0, Q0);

		std::vector<XMFLOAT3> S1(numBones);
		std::vector<XMFLOAT3> P1(numBones);
		std::vector<XMFLOAT4> Q1(numBones);

		auto clip1 = mAnimations.find(nextAnimName);
		clip1->second.Interpolate(nextTimePos, S0, P0, Q0);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		for (UINT i = 0; i < numBones; ++i)
		{
			XMVECTOR s0 = XMLoadFloat3(&S0[i]);
			XMVECTOR s1 = XMLoadFloat3(&S1[i]);

			XMVECTOR p0 = XMLoadFloat3(&P0[i]);
			XMVECTOR p1 = XMLoadFloat3(&P1[i]);

			XMVECTOR q0 = XMLoadFloat4(&Q0[i]);
			XMVECTOR q1 = XMLoadFloat4(&Q1[i]);

			XMVECTOR S = XMVectorLerp(s0, s1, blendFactor);
			XMVECTOR P = XMVectorLerp(p0, p1, blendFactor);
			XMVECTOR Q = XMQuaternionSlerp(q0, q1, blendFactor);

			XMStoreFloat4x4(&finalTransforms[i], XMMatrixAffineTransformation(S, zero, Q, P));
		}

		blendFactor += blendSpeed * 2.0f * (nextTimePos - timePos);

		if (blendFactor >= 1.0f)
		{
			currAnimName = nextAnimName;
			nextAnimName.clear();

			timePos = nextTimePos;
		}
	}
}
