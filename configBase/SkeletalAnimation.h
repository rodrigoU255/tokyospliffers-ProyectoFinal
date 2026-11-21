#pragma once

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Bone.h"
#include "Model.h"

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class SkeletalAnimation
{
public:
	SkeletalAnimation() = default;

	SkeletalAnimation(const std::string& animationPath, Model* model, unsigned int animationIndex = 0)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		if (!scene || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::ANIMATION:: " << importer.GetErrorString() << " (" << animationPath << ")" << std::endl;
			return;
		}
		if (scene->mNumAnimations == 0)
		{
			std::cout << "ERROR::ANIMATION:: El archivo cargo bien, pero tiene 0 animaciones: " << animationPath << std::endl;
			return;
		}

		if (animationIndex >= scene->mNumAnimations)
			animationIndex = 0;

		auto animation = scene->mAnimations[animationIndex];
		m_Duration = (float)animation->mDuration;
		m_TicksPerSecond = (animation->mTicksPerSecond != 0) ? (float)animation->mTicksPerSecond : 25.0f;

		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		m_GlobalInverseTransform = AssimpGLMHelpers::ConvertMatrixToGLMFormat(globalTransformation);

		ReadHierarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);
	}

	Bone* FindBone(const std::string& name)
	{
		for (auto& bone : m_Bones)
		{
			if (bone.GetBoneName() == name)
				return &bone;
		}
		return nullptr;
	}

	inline float GetTicksPerSecond() const { return m_TicksPerSecond; }
	inline float GetDuration() const { return m_Duration; }
	inline const AssimpNodeData& GetRootNode() const { return m_RootNode; }
	inline const std::map<std::string, BoneInfo>& GetBoneIDMap() const { return m_BoneInfoMap; }
	inline const glm::mat4& GetGlobalInverseTransform() const { return m_GlobalInverseTransform; }

private:
	float m_Duration = 0.0f;
	float m_TicksPerSecond = 25.0f;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::map<std::string, BoneInfo> m_BoneInfoMap;
	glm::mat4 m_GlobalInverseTransform = glm::mat4(1.0f);

	void ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		int size = animation->mNumChannels;
		auto& boneInfoMap = model.GetBoneInfoMap();
		int& boneCount = model.GetBoneCount();

		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				if (boneCount >= 100) continue;
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
		}

		m_BoneInfoMap = boneInfoMap;
	}

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (unsigned int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHierarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}
};