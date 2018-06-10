#pragma once
#include <string>

namespace FishEngine
{
	class ScriptSystem
	{
	public:
		static ScriptSystem& GetInstance()
		{
			static ScriptSystem instance;
			return instance;
		}
		
		void Init();
		void Start();
		void Update();
		void Clean();
		void Reload();
		
		void AddScriptDir(const std::string& path);
		
	protected:
		ScriptSystem() = default;
	};
}
