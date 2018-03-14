#pragma once

struct NVGcontext;

namespace FishEngine
{
	class UISystem
	{
	public:
		static UISystem& GetInstance()
		{
			static UISystem instance;
			return instance;
		}
		
		void Init();
//		void Start();
		void BeginDraw();
		void Update();
		void AfterDraw();
		void Clean();
		
	private:
		UISystem() = default;
		
		NVGcontext* m_Context = nullptr;
	};
}
