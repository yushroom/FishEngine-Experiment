#pragma once

namespace FishEngine
{
	class AnimationSystem
	{
	public:

		static AnimationSystem& GetInstance()
		{
			static AnimationSystem instance;
			return instance;
		}

		//void Init();
		void Start();
		void Update();
		//void Clean();

	private:
		AnimationSystem() = default;
	};
}