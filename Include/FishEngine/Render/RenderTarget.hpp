#pragma once

#include "RenderBuffer.hpp"

namespace FishEngine
{

	class RenderTarget
	{
	public:
		RenderTarget() = default;

		void SetColorBufferOnly(ColorBuffer* colorBuffer);
		void SetDepthBufferOnly(DepthBuffer* depthBuffer);

		void Set(ColorBuffer* colorBuffer,
				 DepthBuffer* depthBuffer);

		void Set(ColorBuffer* colorBuffer1,
				 ColorBuffer* colorBuffer2,
				 ColorBuffer* colorBuffer3,
				 DepthBuffer* depthBuffer);

		void Attach();
		void AttachForRead();
		void Detach();

		unsigned int GetGLNativeFBO() const
		{
		    return m_fbo;
		}

		ColorBuffer* GetColorBuffer(int index = 0) {
			assert(0 <= index && index <= 3);
			return m_colorBuffers[0];
		}

	private:
		bool            m_useDepthBuffer = true;
		uint32_t        m_activeColorBufferCount = 1;
		ColorBuffer*  	m_colorBuffers[3];
		DepthBuffer*  	m_depthBuffer;
		unsigned int    m_fbo = 0;

		bool m_Initialized = false;
		void Init();
	};
}
