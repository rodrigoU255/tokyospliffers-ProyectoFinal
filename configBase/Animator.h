#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "SkeletalAnimation.h"

class Animator
{
public:
	Animator(SkeletalAnimation* animation)
	{
		m_CurrentTime = 0.0f;
		m_CurrentAnimation = animation;
		m_FinalBoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	void UpdateAnimation(float dt)
	{
		m_DeltaTime = dt;
		if (m_CurrentAnimation)
		{
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void PlayAnimation(SkeletalAnimation* pAnimation)
	{
		// Solo reinicia el tiempo si cambias a una animación distinta (ej. Idle -> Run)
		if (m_CurrentAnimation != pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}
	}

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* bone = m_CurrentAnimation->FindBone(nodeName);
		if (bone)
		{
			bone->Update(m_CurrentTime);
			nodeTransform = bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap.at(nodeName).id;
			glm::mat4 offset = boneInfoMap.at(nodeName).offset;
			if (index < 100)
			{
				m_FinalBoneMatrices[index] =
					m_CurrentAnimation->GetGlobalInverseTransform() * globalTransformation * offset;
			}
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	const std::vector<glm::mat4>& GetFinalBoneMatrices() const
	{
		return m_FinalBoneMatrices;
	}


	void SendMatricesToShader(Shader& shader)
	{
		for (unsigned int i = 0; i < m_FinalBoneMatrices.size(); ++i)
		{
			std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, name.c_str()), 1, GL_FALSE, &m_FinalBoneMatrices[i][0][0]);
		}
	}
private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	SkeletalAnimation* m_CurrentAnimation = nullptr;
	float m_CurrentTime = 0.0f;
	float m_DeltaTime = 0.0f;
};