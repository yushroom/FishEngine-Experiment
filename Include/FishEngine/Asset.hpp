#pragma once
#include "Object.hpp"
//#include "Render/Mesh.hpp"
//#include "Render/Material.hpp"
//#include "Render/Shader.hpp"

#include <set>

namespace FishEngine
{
	inline bool IsAsset(Object* object)
	{
		if (object == nullptr)
			return false;
		int classID = object->GetClassID();
//		return classID == Mesh::ClassID ||
//			   classID == Material::ClassID ||
//			   classID == Shader::ClassID;
		return classID == 43 || classID == 21 || classID == 48;
	}

	class AssetManager
	{
	public:
		AssetManager(AssetManager&) = delete;
		AssetManager& operator=(AssetManager&) = delete;

		~AssetManager()
		{
			ClearAll();
		}

		static AssetManager& GetInstance()
		{
			static AssetManager instance;
			return instance;
		}

		void AddAsset(Object* asset);
		void RemoveAsset(Object* asset);

		void ClearAll();

	private:
		AssetManager() = default;
//		std::map<int, std::set<Ob>>
		std::set<Object*> m_Assets;
		bool m_Cleaning = false;
	};
}