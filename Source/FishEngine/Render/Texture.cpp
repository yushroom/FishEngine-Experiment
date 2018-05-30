#include <FishEngine/Render/Texture.hpp>

#include <cassert>

#include <FishEngine/Render/GLEnvironment.hpp>
#include <FishEngine/Debug.hpp>
#include <FishEngine/Math/Mathf.hpp>
#include <FishEngine/Render/TextureSampler.hpp>

namespace FishEngine
{
	void Texture::BindSampler()
	{
		assert(m_uploaded);
		assert(m_GLNativeTexture != 0);
		const auto& sampler = TextureSampler::GetSampler(m_filterMode, m_wrapMode);
		glBindSampler(m_GLNativeTexture, sampler.m_nativeGLSampler);
	}

	Texture::~Texture()
	{
		if (m_GLNativeTexture != 0)
			glDeleteTextures(1, &m_GLNativeTexture);
		AssetManager::GetInstance().RemoveAsset(this);
	}
}
