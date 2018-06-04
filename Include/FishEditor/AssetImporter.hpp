#pragma once

#include <cassert>

#include <FishEngine/Object.hpp>
#include <unordered_map>
#include <map>

namespace FishEditor
{
	class AssetMetaData
	{
	public:
		enum { ClassID = 1028 };
	};

	class AssetImporter : public FishEngine::Object
	{
	public:
		DeclareObject(AssetImporter, 1003);
		
		AssetImporter(int classID, const char* className) : Object(classID, className) { }
		virtual ~AssetImporter();

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;

		const std::string& GetAssetPath() const { return m_AssetPath; }
		void SetAssetPath(const std::string& value);

		const std::string& GetGUID() const { return m_GUID; }
		void SetGUID(const std::string& value) { m_GUID = value; }

		uint32_t GetAssetTimeStamp() const { return m_AssetTimeStamp; }
		void SetAssetTimeStamp(uint32_t value) { m_AssetTimeStamp = value; }

		static AssetImporter* GetAtPath(std::string path);
		static AssetImporter* GetByGUID(const std::string& guid);
		
		static std::string CorrectAssetPath(const std::string& path);
		
		void SaveAndReimport();

		static void AddImporter(AssetImporter* importer, const std::string& guid);

		static const std::unordered_map<std::string, AssetImporter*>& GetGUIDToImporter()
		{
			return s_GUIDToImporter;
		}

		std::string GetFullPath() const;

		virtual void Import() = 0;

		FishEngine::Object* GetMainAsset()
		{
			if (m_MainAsset == nullptr)
			{
				this->Import();
			}
			assert(m_MainAsset != nullptr);
			return m_MainAsset;
		}

		const std::map<int64_t, FishEngine::Object*>& GetFileIDToObject() const
		{
			return m_FileIDToObject;
		}

		Object* GetObjectByFileID(int64_t fileID)
		{
			auto it = m_FileIDToObject.find(fileID);
			if (it != m_FileIDToObject.end())
				return it->second;
			return nullptr;
		}

	protected:
		friend class AssetDatabase;
		friend class MetaInputArchive;

		static std::unordered_map<std::string, AssetImporter*> s_GUIDToImporter;

		std::string m_AssetPath;
		std::string m_GUID;
		uint32_t m_AssetTimeStamp;

		bool m_Imported = false;
		std::map<int64_t, FishEngine::Object*> m_FileIDToObject;

		// For example an imported model has a game object as its root and several Meshes
		// and child game objects in expanded state. The root game object is the main asset
		// in this case.
		FishEngine::Object* m_MainAsset = nullptr;
		
		int64_t m_MainObjectFileID = 0;
	};
}
