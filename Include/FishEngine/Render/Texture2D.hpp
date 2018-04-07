#pragma once

#include "Texture.hpp"
#include <vector>

namespace FishEngine
{
	class Texture2D : public Texture
	{
	public:
		InjectClassName(Texture2D, 28);
		Texture2D() : Texture(ClassID, ClassName)
		{
		}

		Texture2D(int width, int height, TextureFormat format, const uint8_t* data, int byteCount = -1);

		// The format of the pixel data in the texture (Read Only).
		TextureFormat format() const
		{
			return m_format;
		}

		// How many mipmap levels are in this texture (Read Only).
		uint32_t mipmapCount() const
		{
			return m_mipmapCount;
		}

		// Get a small texture with all white pixels.
		static Texture2D* whiteTexture();
		static Texture2D* blackTexture();


	protected:
		virtual void UploadToGPU() override;

	protected:
//		friend class FishEditor::TextureImporter;
//		friend class FishEditor::DDSImporter;

		std::vector<std::uint8_t> m_data;

		// The format of the pixel data in the texture (Read Only).
		TextureFormat 	m_format;

		// How many mipmap levels are in this texture (Read Only).
		uint32_t		m_mipmapCount;
	};
}
