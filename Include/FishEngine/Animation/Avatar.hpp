#pragma once

#include "../Object.hpp"
#include "../Asset.hpp"
#include "../Math/Matrix4x4.hpp"
#include <map>
#include <vector>

namespace FishEngine
{
	class FE_EXPORT Avatar : public Object
	{
	public:
		DeclareObject(Avatar, 90);
		Avatar() : Object(Avatar::ClassID, ClassName)
		{
			AssetManager::GetInstance().AddAsset(this);
		}

		std::map<std::string, int> m_boneToIndex;
		std::map<int, std::string> m_indexToBone;
		std::vector<Matrix4x4> m_matrixPalette;
	};

	class MatrixPalette
	{
	public:
		mutable std::vector<Matrix4x4> m_matrixPalette;
		void UpdateMatrixPalette() const;
	};
}
