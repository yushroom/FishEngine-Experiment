#pragma once

#include <FishEngine/GameObject.hpp>

namespace FishEditor
{
	class EditorUtility final
	{
	public:
		static FishEngine::GameObject* CreateGameObjectWithHideFlags(const std::string& name, FishEngine::HideFlags flags)
		{
			auto go = new FishEngine::GameObject(name);
			go->SetHideFlags(flags);
			return go;
		}
	};
}