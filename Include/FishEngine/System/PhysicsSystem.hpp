#pragma once

namespace FishEngine
{
	class PhysicsSystem
	{
	public:

		static PhysicsSystem& GetInstance()
		{
			static PhysicsSystem instance;
			return instance;
		}

		void Init();
		void Start();
		void FixedUpdate();
		void Clean();


	private:
		PhysicsSystem() = default;
	};
}