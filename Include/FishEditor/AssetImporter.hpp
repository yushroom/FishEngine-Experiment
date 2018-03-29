#pragma once

#include <FishEngine/Object.hpp>
#include <unordered_map>

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
		constexpr static int ClassID = 1003;
		constexpr static const char* ClassName = "AssetImporter";
		
		AssetImporter(int classID, const char* className) : Object(classID, className) { }
		virtual ~AssetImporter() = default;

		// noncopyable
		AssetImporter(AssetImporter const &) = delete;
		AssetImporter& operator=(AssetImporter const &) = delete;

		const std::string& GetAssetPath() const { return m_AssetPath; }
		void SetAssetPath(const std::string& value) { m_AssetPath = value; }

		const std::string& GetGUID() const { return m_GUID; }
		void SetGUID(const std::string& value) { m_GUID = value; }

		uint32_t GetAssetTimeStamp() const { return m_AssetTimeStamp; }
		void SetAssetTimeStamp(uint32_t value) { m_AssetTimeStamp = value; }

		static AssetImporter* GetAtPath(const std::string& path);
		static AssetImporter* GetByGUID(const std::string& guid);
		
		void SaveAndReimport();

		static void AddImporter(AssetImporter* importer, const std::string& guid);

		static const std::unordered_map<std::string, AssetImporter*>& GetGUIDToImporter()
		{
			return s_GUIDToImporter;
		}

	protected:
		static std::unordered_map<std::string, AssetImporter*> s_GUIDToImporter;

		std::string m_AssetPath;
		std::string m_GUID;
		uint32_t m_AssetTimeStamp;
	};
}
