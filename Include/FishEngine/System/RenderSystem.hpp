namespace FishEngine
{
	class RenderSystem
	{
	public:
		static RenderSystem& GetInstance()
		{
			static RenderSystem instance;
			return instance;
		}
		
		void Update();
		
	private:
		RenderSystem() = default;
		
		RenderSystem(RenderSystem&) = delete;
		RenderSystem& operator=(RenderSystem&) = delete;
	};
}
