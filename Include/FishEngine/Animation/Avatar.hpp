#pragma once

#include "../Object.hpp"
#include "../Asset.hpp"
#include "../Math/Matrix4x4.hpp"
#include <map>
#include <vector>

#include "HumanDescription.hpp"
#include "HumanBodyBones.hpp"


namespace FishEngine
{
	class GameObject;
	class Transform;
	
	class FE_EXPORT Avatar : public Object
	{
	public:
		DeclareObject(Avatar, 90);
		Avatar() : Object(Avatar::ClassID, ClassName)
		{
			AssetManager::GetInstance().AddAsset(this);
		}

		std::map<std::string, int> m_boneToIndex;
		std::vector<std::string> m_indexToBoneName;
		
//		std::map<std::string, Matrix4x4> m_BindPoses;
		
		// human
		std::map<std::string, HumanBodyBones> m_BoneNameToHuman;
//		std::vector<Matrix4x4> m_BindPoses2;
		
		
		static Avatar*
		BuildHumanAvatar(GameObject* go, const HumanDescription& humanDescription);
		
		bool IsHuman() const  { return m_IsHuman; }
		
		void GetHumanSkeleton();
		
		Transform* m_HumanSkeleton[(int)HumanBodyBones::LastBone] = {nullptr};
		
		HumanDescription m_HumanDescription;
		SkeletonBone m_HumanSkeletonBones[(int)HumanBodyBones::LastBone];
		
	private:
		bool m_IsHuman = false;
	};

//	class MatrixPalette
//	{
//	public:
//		mutable std::vector<Matrix4x4> m_matrixPalette;
//		void UpdateMatrixPalette() const;
//	};
}
