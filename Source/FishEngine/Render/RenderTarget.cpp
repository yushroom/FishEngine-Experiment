#include <FishEngine/Render/RenderTarget.hpp>

#include <cassert>

#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Render/Texture.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Render/RenderBuffer.hpp>


namespace FishEngine
{
	RenderTarget::~RenderTarget()
	{
		if (m_fbo != 0)
			glDeleteFramebuffers(1, &m_fbo);
	}

	void RenderTarget::SetColorBufferOnly(ColorBuffer* colorBuffer)
	{
		m_activeColorBufferCount = 1;
		m_colorBuffers[0] = colorBuffer;
		m_useDepthBuffer = false;
		Init();
	}

	void RenderTarget::SetDepthBufferOnly(DepthBuffer* depthBuffer)
	{
		m_activeColorBufferCount = 0;
		m_depthBuffer = depthBuffer;
		Init();
	}

	void RenderTarget::Set(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer)
	{
		m_activeColorBufferCount = 1;
		m_colorBuffers[0] = colorBuffer;
		m_depthBuffer = depthBuffer;

		Init();
	}

	void RenderTarget::Set(ColorBuffer* colorBuffer1,
						   ColorBuffer* colorBuffer2,
						   ColorBuffer* colorBuffer3,
						   DepthBuffer* depthBuffer)
	{
		m_activeColorBufferCount = 3;
		m_colorBuffers[0] = colorBuffer1;
		m_colorBuffers[1] = colorBuffer2;
		m_colorBuffers[2] = colorBuffer3;
		m_depthBuffer = depthBuffer;

		Init();
	}

	void RenderTarget::Attach()
	{
		assert(m_Initialized);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void RenderTarget::AttachForRead()
	{
		assert(m_Initialized);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	}

	void RenderTarget::Detach()
	{
		assert(m_Initialized);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderTarget::Init()
	{
		assert(!m_Initialized);
		m_Initialized = true;
		LogInfo("RenderTarget::Init");
		glGenFramebuffers(1, &m_fbo);
		assert(m_fbo > 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		if (m_useDepthBuffer)
		{
			if (m_depthBuffer->m_useStencil)
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_depthBuffer->GetNativeTexturePtr(), 0);
			else
				glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthBuffer->GetNativeTexturePtr(), 0);
		}

		if (m_activeColorBufferCount > 0)
		{
			for (uint32_t i = 0; i < m_activeColorBufferCount; ++i)
			{
				auto rt = m_colorBuffers[i]->GetNativeTexturePtr();
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rt, 0);
			}
			GLenum attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_activeColorBufferCount, attachments);
		}


		auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE)
		{
			LogError("Framebuffer is not complete!");
			std::string str;
#define TEMP_CASE(c) case c: str = #c; break;
			switch (result)
			{
				TEMP_CASE(GL_FRAMEBUFFER_UNDEFINED)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
				TEMP_CASE(GL_FRAMEBUFFER_UNSUPPORTED)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
				TEMP_CASE(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
				default:
					str = "UNKNOWN";
					break;
			}
			LogError(str);
#undef TEMP_CASE
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glCheckError();
	}
}
