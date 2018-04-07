#pragma once
//#include "GLEnvironment.hpp"
#include "Texture.hpp"

namespace FishEngine {

	enum class RenderTextureFormat
	{
		ARGB32, // 4 channel, 8 bit per channel
		Dpeth,  //
		Shadowmap,
	};

	class RenderTexture : public Texture
	{
	public:
		InjectClassName(RenderTexture, 84);

		RenderTexture() : Texture(ClassID, ClassName)
		{

		}
		~RenderTexture();


		uint32_t FBO() const
		{
			return m_FBO;
		}

		uint32_t depthBuffer() const
		{
			return m_depthBuffer;
		}

		uint32_t colorBuffer() const
		{
			return m_GLNativeTexture;
		}

		void Resize(const int newWidth, const int newHeight) override ;

	private:
		RenderTextureFormat m_format;
		uint32_t 			m_FBO = 0;
		uint32_t 			m_depthBuffer = 0;
	};
}
