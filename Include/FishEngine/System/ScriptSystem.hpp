#pragma once

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
		
	protected:
		ScriptSystem() = default;
	};
}
