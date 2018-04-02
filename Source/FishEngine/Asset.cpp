#include <FishEngine/Asset.hpp>

namespace FishEngine
{
	void AssetManager::AddAsset(Object* asset)
	{
		assert(!m_Cleaning);
		bool flag = IsAsset(asset);
		assert(flag);
		if (flag)
		{
			m_Assets.insert(asset);
		}
	}

	void AssetManager::RemoveAsset(Object* asset)
	{
		if (m_Cleaning)
			return;
		auto pos = m_Assets.find(asset);
		int size = m_Assets.size();
		assert(pos != m_Assets.end());
		if (pos != m_Assets.end())
		{
			m_Assets.erase(pos);
		}
	}

	void AssetManager::ClearAll()
	{
		m_Cleaning = true;
		for (Object* asset : m_Assets)
		{
			int size = m_Assets.size();
			delete asset;
		}
		m_Assets.clear();
		m_Cleaning = false;
	}
}
