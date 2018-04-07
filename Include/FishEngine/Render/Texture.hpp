#pragma once

#include "../Object.hpp"
#include "TextureProperty.hpp"
#include "../Asset.hpp"

namespace FishEditor
{
	class TextureImporter;
}

namespace FishEngine
{

	class Texture : public Object
	{
	public:
		InjectClassName(Texture, 27);

		Texture(int classID, const char* className) : Object(classID, className)
		{
			AssetManager::GetInstance().AddAsset(this);
		}

		virtual ~Texture();

		int width() const
		{
			return m_width;
		}

		int height() const
		{
			return m_height;
		}

		unsigned int GetNativeTexturePtr()
		{
			if (!m_uploaded)
				UploadToGPU();
			if (m_GLNativeTexture == 0) abort();
			return m_GLNativeTexture;
		}

		// Filtering mode of the texture.
		FilterMode filterMode() const
		{
			return m_filterMode;
		}

		void setFilterMode(const FilterMode filterMode)
		{
			m_filterMode = filterMode;
			BindSampler();
		}

		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode wrapMode() const
		{
			return m_wrapMode;
		}

		void setWrapMode(const TextureWrapMode wrapMode)
		{
			m_wrapMode = wrapMode;
			BindSampler();
		}

		TextureDimension dimension() const
		{
			return m_dimension;
		}

		void setDimension(TextureDimension dimension)
		{
			m_dimension = dimension;
		}

		virtual void Resize(int, int)
		{
			abort();
		}

		virtual void Deserialize(InputArchive& archive) override {};
		virtual void Serialize(OutputArchive& archive) const override {};

	protected:
		friend class FishEditor::TextureImporter;

		virtual void UploadToGPU() { m_uploaded = true; };
		void BindSampler();


	protected:
		int            m_height;
		int            m_width;

		// Anisotropic filtering level of the texture.
		int                 m_anisoLevel;

		// Dimensionality (type) of the texture (Read Only).
		TextureDimension    m_dimension = TextureDimension::Unknown;

		// Filtering mode of the texture.
		FilterMode          m_filterMode = FilterMode::Trilinear;

		// Wrap mode (Repeat or Clamp) of the texture.
		TextureWrapMode     m_wrapMode = TextureWrapMode::Repeat;

		// OpenGL
		mutable uint32_t	m_GLNativeTexture = 0;

		mutable bool		m_uploaded = false;
	};
}