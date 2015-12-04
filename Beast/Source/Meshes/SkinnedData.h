#ifndef SKINNEDDATA_H
#define SKINNEDDATA_H

#include "../Helper.h"
#include <map>

///<summary>
/// A Keyframe defines the bone transformation at an instant in time.
///</summary>
struct Keyframe
{
	Keyframe();
	~Keyframe();

    float TimePos;
	XMFLOAT3 Translation;
	XMFLOAT3 Scale;
	XMFLOAT4 RotationQuat;
};

///<summary>
/// A BoneAnimation is defined by a list of keyframes.  For time
/// values inbetween two keyframes, we interpolate between the
/// two nearest keyframes that bound the time.  
///
/// We assume an animation always has two keyframes.
///</summary>
struct BoneAnimation
{
	float GetStartTime()const;
	float GetEndTime()const;

	void Interpolate(float t, XMFLOAT3& S, XMFLOAT3& P, XMFLOAT4& Q)const;

	std::vector<Keyframe> Keyframes; 	

};

///<summary>
/// Examples of AnimationClips are "Walk", "Run", "Attack", "Defend".
/// An AnimationClip requires a BoneAnimation for every bone to form
/// the animation clip.    
///</summary>
struct AnimationClip
{
	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Interpolate(float t, std::vector<XMFLOAT3>& STransforms, std::vector<XMFLOAT3>& PTransforms, std::vector<XMFLOAT4>& QTransforms)const;

    std::vector<BoneAnimation> BoneAnimations; 	
};

class SkinnedData
{
public:
	SkinnedData();

	// Should be called only and at least Once per Instance (during initialization??)
	void SetCurrAnim(std::string& clipName);

	void SetNextAnim(std::string& clipName);
	void NextDefaultAnim() { SetNextAnim(defaultAnimName); }
	float nextTimePos;
	float blendSpeed = 1.0f;

	UINT BoneCount()const;

	float GetClipStartTime()const;
	float GetClipEndTime()const;

	void Set(
		std::vector<int>& boneHierarchy,
		std::vector<std::string>& mBoneNameHierarchy,
		std::vector<XMFLOAT4X4>& boneOffsets,
		std::map<std::string, AnimationClip>& animations);

	std::map<std::string, AnimationClip> getAnimations() { return mAnimations; }
	void setAnimations(const std::map<std::string, AnimationClip>& anims) { mAnimations = anims; }

	std::vector<XMFLOAT4X4>& GetBoneOffsets() { return mBoneOffsets; }

	 // In a real project, you'd want to cache the result if there was a chance
	 // that you were calling this several times with the same clipName at 
	 // the same timePos.
	bool GetFinalTransforms(float timePos, std::vector<XMFLOAT4X4>& finalTransforms);
	int GetBoneIndex(std::string &boneName)
	{
		for (UINT i = 0; i < mBoneNameHierarchy.size(); ++i)
		{
			if (mBoneNameHierarchy[i] == boneName)
				return i;
		}

		return -1;
	}

	std::string& GetBoneName(int i)
	{
		std::string tmp;

		if (i > mBoneNameHierarchy.size())
			return tmp;

		return mBoneNameHierarchy[i];
	}

private:
	void BlendAnims(float timePos, std::vector<XMFLOAT4X4>& finalTransforms);

	std::string currAnimName;
	std::string nextAnimName;
	std::string defaultAnimName;
	float blendFactor = 0.0f;

    // Gives parentIndex of ith bone.
	std::vector<int> mBoneHierarchy;
	std::vector<std::string> mBoneNameHierarchy;

	std::vector<XMFLOAT4X4> mBoneOffsets;
   
	std::map<std::string, AnimationClip> mAnimations;
};
 
#endif // SKINNEDDATA_H